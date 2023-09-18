#include "segment_store.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include <zconf.h>
#include <schedule.hpp>
#include <stdio.h>
#include <iostream>
// #include <gflags/gflags.h>
#include <glog/logging.h>

bool LogSegmentStore::Init(const paxosme::LogStorage::LogStorageOptions &options)
{
    if (!db_path_.empty())
    {
        LOG(ERROR) << "db path = " << db_path_ << " already exists.";
        return false;
    }
    
    if(options.indexKeyLength == 0)
    {
        LOG(ERROR) << "indexKeyLength cannot be zero.";
        return false;
    }

    db_path_ = options.dbPath + (options.dbPath.back() == '/' ? "" : "/") + SEGMENT_STORE_DIR;
    index_key_length_ = options.indexKeyLength;

    if (!CreateIfNotExists(db_path_))
    {
        LOG(ERROR) << "Create db path failed." << db_path_;
        return false;
    }

    const std::string &path = db_path_ + (options.dbPath.back() == '/' ? "" : "/") + METADATA_FILE;
    auto meta_data_cpath = path.c_str();

    meta_fd_ = open(meta_data_cpath,
                    O_CREAT | O_RDWR /* creat if not exists and open for read&write */,
                    S_IREAD | S_IWRITE /* Read, Write by owner */
    );

    if (meta_fd_ == -1)
    {
        // open meta data file failed
        LOG(ERROR) << "Open meta data file failed." << meta_data_cpath;
        return false;
    }

    // lseek meta file to 0
    off_t head_pos = lseek(meta_fd_, 0, SEEK_SET);

    if (head_pos == -1)
    {
        // lseek failed
        LOG(ERROR) << "Meta data file seek failed.";
        return false;
    }

    // read first SEGMENT_ID as segment id
    // todo II: read file with c++ style
    ssize_t read_len = read(meta_fd_, &cur_segment_id_, sizeof(SEGMENT_ID));
    if (read_len == 0)
    {
        // means first time init if read SEGMENT_ID empty, set 0 to cur_segment_id_
        cur_segment_id_ = 0;
    }
    else if (read_len != (ssize_t)sizeof(SEGMENT_ID))
    {
        // read SEGMENT_ID failed
        LOG(ERROR) << "Read SEGMENT_ID failed.";
        return false;
    }

    // read next crc and check equality with calculation result
    uint32_t read_check_sum = 0;
    read_len = read(meta_fd_, &read_check_sum, sizeof(CHECKSUM));
    if (read_len == (ssize_t)sizeof(CHECKSUM))
    {
        CHECKSUM cal_checksum = crc32(0, (const Bytef *)(&cur_segment_id_), sizeof(SEGMENT_ID));

        if (cal_checksum != read_check_sum)
        {
            // checksum not match
            LOG(ERROR) << "Checksum not match.";
            return false;
        }
    }

    FD fd;
    bool st = OpenSegment(cur_segment_id_, fd);

    if (!st)
    {
        // on err, open segment file failed
        LOG(ERROR) << "Open segment file failed.";
        return false;
    }

    cur_segment_fd_ = fd;
    

    // padding
    segment_max_size_ = options.segmentMaxSize;
    int padding_result = PaddingIfNewSegment();
    if (padding_result < 0)
    {
        // on err, padding failed
        LOG(ERROR) << "New segment padding failed.";
        return false;
    }
    else if (padding_result == 1)
    {
        // seek to begin
        int offset = lseek(cur_segment_fd_, 0, SEEK_SET);
        cur_segment_offset_ = 0;
        if (offset != 0)
        {
            // on err, seek failed
            LOG(ERROR) << "Segment seek failed.";
            return false;
        }
    }
    else
    {
        // replay latest segment and get offset
        off_t offset = 0;
        bool replay_res = ReplaySegment(cur_segment_id_, offset);
        cur_segment_offset_ = offset;
        assert(replay_res && cur_segment_offset_ > 0);
    }
    return true;
}

bool LogSegmentStore::Read(const SegmentIndex &segment_index, std::string &index_key, std::string &value_in_segment)
{
    std::unique_lock<std::mutex> lock(mutex_);
    SEGMENT_ID segment_id;
    off_t offset;
    CHECKSUM parse_checksum;
    ParseSegmentIndex(segment_index, segment_id, offset, parse_checksum);

    LOG(INFO) << "segment_id:" << segment_id << ", offset:" << offset << ", parse_checksum:" << parse_checksum;
    
    FD segment_fd;
    bool st = OpenSegment(segment_id, segment_fd);
    if (!st)
    {
        // on err, open segment failed
        LOG(ERROR) << "Open segment failed.";
        return false;
    }

    // off_t segment_size = lseek(segment_fd, 0, SEEK_END);
    // LOG(INFO) << "segment_size:" << segment_size;
    
    if (lseek(segment_fd, offset, SEEK_SET) != offset)
    {
        // on err, seek failed
        LOG(ERROR) << "Seek segment failed. offset: " << offset << " segment_id: " << segment_id;
        return false; 
    }

    
    size_t kv_size;
    // todo II: read file with c++ style
    size_t read_len = read(segment_fd, &kv_size, sizeof(size_t));
    LOG(INFO) << "kv_size: " << kv_size;

    if (read_len != sizeof(size_t))
    {
        // on err, read kv_size failed
        LOG(ERROR) << "Read segment failed.";
        return false;
    }

    std::string keyValue(kv_size, '\0');
    read_len = read(segment_fd, (void *)keyValue.c_str(), index_key_length_);
    LOG(INFO) << "keyValue: " << keyValue;

    if (read_len != index_key_length_)
    {
        // on err, read key_in_segment failed
        LOG(ERROR) << "read key length: " << read_len << " expected key length: " << index_key_length_;
        return false;
    }


    const std::string& key_in_segment = keyValue.substr(0, index_key_length_);
    LOG(INFO) << "key_in_segment: " << key_in_segment;

    if (key_in_segment != index_key)
    {
        // on err, key_in_segment not match
        LOG(ERROR) << "key_in_segment not match. key_in_segment: " << key_in_segment << " index_key: " << index_key;
        return false;
    }

    size_t value_len = kv_size - index_key_length_;
    read_len = read(segment_fd, (void *)(keyValue.c_str() + index_key_length_), value_len);
    LOG(INFO) << "keyValue: " << keyValue;
    
    if (read_len != value_len)
    {
        // on err, read value failed
        LOG(ERROR) << "read len not matched. read_len: " << read_len << " expected len: " << value_len;
        return false;
    }
    CHECKSUM cal_checksum = crc32(0, (const uint8_t *)keyValue.c_str(), kv_size);

    if (cal_checksum != parse_checksum)
    {
        // on err, calculated checksum not matched
        LOG(ERROR) << "calculated checksum not matched. cal_checksum: " << cal_checksum << " parse_checksum: " << parse_checksum;
        return false;
    }

    value_in_segment = keyValue.substr(index_key_length_, value_len);

    LOG(INFO) << "value in segment: " << value_in_segment;


    return true;
}

bool LogSegmentStore::Append(const std::string &key, const std::string &value, SegmentIndex &segment_index)
{
    std::unique_lock<std::mutex> lock(mutex_);

    FD fd = -1;
    SEGMENT_ID segment_id;
    off_t offset;

    const size_t value_size = value.size();
    const size_t key_value_size = index_key_length_ + value_size;
    // buffer_length = size(length) + key_value_size
    const size_t buffer_length = sizeof(size_t) + key_value_size;

    if (buffer_length > segment_max_size_)
    {
        // on err, too big buffer to write
        return false;
    }

    // NewSegment will set the new segement and offset pos if need
    if (cur_segment_offset_ + value_size > cur_segment_file_size_)
    {
        LOG(INFO) << "cur_segment_file_size :" << cur_segment_file_size_;
        // truncate current segment as not enough space for new log
        if (ftruncate(cur_segment_fd_, cur_segment_offset_) != 0)
        {
            // on err, truncate segment failed
            return false;
        }
        
        // close current segment
        close(cur_segment_fd_);
        bool ready = NewSegment(value_size);

        if (!ready)
        {
            // segment update failed.
            return false;
        }
        else {
            cur_segment_offset_ = 0;
        }
    }
    
    char buffer[buffer_length];
    memcpy(buffer, &key_value_size, sizeof(size_t));
    memcpy(buffer + sizeof(size_t), key.c_str(), index_key_length_);
    memcpy(buffer + sizeof(size_t) + index_key_length_, value.c_str(), value_size);

    std::string metaStr(buffer, buffer_length);
    LOG(INFO) << "key_value_size:" << key_value_size;
    LOG(INFO) << "metaStr:" << metaStr;
    LOG(INFO) << "buffer_length:" << buffer_length << ", key_value_size:" << key_value_size << ", index_key_length_:" << index_key_length_ << ", value_size:" << value_size;
    LOG(INFO) << "cur_segment_offset_: " << cur_segment_offset_; 
    if (lseek(cur_segment_fd_, cur_segment_offset_, SEEK_SET) != cur_segment_offset_)
    {
        // on err
        LOG(ERROR) << "Seek segment failed.";
        return false;
    }
    
    // todo II: read file with c++ style
    size_t write_length = write(cur_segment_fd_, buffer, buffer_length);
    LOG(INFO) << "write_length:" << write_length;

    if (write_length != buffer_length)
    {
        // write segment failed
        return false;
    }
    
    uint32_t checksum = crc32(0, (const uint8_t *)(buffer + sizeof(size_t)), key_value_size);
    ToSegmentIndex(cur_segment_id_, cur_segment_offset_, checksum, segment_index);

    cur_segment_offset_ += buffer_length;

    LOG(INFO) << "cur_segment_offset_:" << cur_segment_offset_;

    return true;
}

bool LogSegmentStore::Remove(const SegmentIndex &segment_index)
{
    SEGMENT_ID segment_id;
    off_t offset;
    CHECKSUM parse_checksum;
    ParseSegmentIndex(segment_index, segment_id, offset, parse_checksum);

    FD segment_fd;
    bool st = OpenSegment(segment_id, segment_fd);
    if (!st)
    {
        // on err, open segment failed
        return false;
    }
    

    if (!lseek(segment_fd, offset, SEEK_SET))
    {
        // on err, seek failed
        close(segment_fd);
        return false; 
    }

    size_t kv_size;
    // todo II: read file with c++ style
    size_t read_len = read(segment_fd, &kv_size, sizeof(size_t));
    if (read_len != sizeof(size_t))
    {
        // on err, read kv_size failed
        close(segment_fd);
        return false;
    }

    off_t next = offset + sizeof(size_t) + kv_size;
    off_t end = lseek(segment_fd, 0, SEEK_END);
    close(segment_fd);
    
    if (next == end)
    {
        // delete this segment in which all logs have been removed
        EventTimeStamp t = STEADY_TIME_NOW;
        const int delayInMilli = 100;
        const std::chrono::duration<int> delay(delayInMilli);
        t.operator+=(delay);

        // 10 is the safe distance, TBD
        auto callback = [this, segment_id] { DeleteSegmentBefore(segment_id - 10); };
        Scheduler::OneInstance()->AddEvent(callback, t, kEVENT_CLEAN_SEGMENT);
        // todo I: loop for segment clean
    }

    return true;
}

SEGMENT_ID LogSegmentStore::GetLastSegmentId()
{
    // todo I: implement this
    return SEGMENT_ID();
}

bool LogSegmentStore::ReplaySegment(const SEGMENT_ID &segment_id, off_t &offset)
{
    SegmentIndex next_index_key;
    SegmentIndex next_log_index;
    bool replay_res = false;
    off_t replay_offset = offset;

    do
    {
        replay_res = ReplayLog(segment_id, replay_offset, next_index_key, next_log_index);
    } while (replay_res);

    if (replay_offset < 0)
    {
        // replay failed
        return false;
    }

    // reach the end of segment
    offset = replay_offset;

    return true;
}

LogSegmentStore *LogSegmentStore::OneInstance()
{   
    static LogSegmentStore logSegmentStore;
    return &logSegmentStore;
}

LogSegmentStore *LogSegmentStore::New()
{   
    return new LogSegmentStore;
}

bool LogSegmentStore::ReplayLog(const SEGMENT_ID &segment_id, off_t &offset, SegmentIndex &index_key, SegmentIndex &log_index)
{
    if (!SegmentExists(segment_id))
    {
        // segment not exists
        offset = -1;
        return false;
    }

    FD fd;
    OpenSegment(segment_id, fd);

    if (fd < 0)
    {
        // open failed
        offset = -1;
        return false;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 0)
    {
        // segment file corrupted
        close(fd);
        offset = -1;
        return false;
    }
    off_t pos = lseek(fd, offset, SEEK_SET);
    if (pos == -1)
    {
        close(fd);
        offset = -1;
        return false;
    }

    size_t length = 0;
    // read first length
    // todo II: read file with c++ style
    ssize_t read_len = read(fd, (char *)&length, sizeof(size_t));
    if (read_len != sizeof(int))
    {
        // todo II: means read to the end if read_len == 0, otherwise needs truncate segment(discard the extra data after pos)
        offset = pos;
        close(fd);
        return false;
    }

    if (length == 0)
    {
        // end of the segment, no need to read more
        offset = pos;
        close(fd);
        return false;
    }

    char index_key_buf[index_key_length_];
    read_len = read(fd, index_key_buf, index_key_length_);

    if (length != index_key_length_)
    {
        // todo II: means read to the end if read_len == 0, otherwise needs truncate segment(discard the extra data after pos)
        return false;
    }

    index_key.append(index_key_buf, index_key_length_);

    // read next [length - index_key_length_] bytes from fd
    char buffer[length - index_key_length_];
    read_len = read(fd, buffer, length - index_key_length_);

    uint32_t checksum = crc32(0, (const uint8_t *)buffer, length - index_key_length_);

    close(fd);
    ToSegmentIndex(segment_id, offset, checksum, log_index);

    return true;
}

bool LogSegmentStore::CreateIfNotExists(const std::string &path)
{
    struct stat path_info;
    const char *cpath = path.c_str();
    if (stat(cpath, &path_info) != 0)
    {
        // not exixts and create
        if (mkdir(cpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH /*Read, Write, Execute by owner and others*/) == -1)
        {
            // on err, create dir failed
            return false;
        }
    }
    else if (!(path_info.st_mode & S_IFDIR))
    {
        // on err, exists but not directory
        return false;
    }
    return true;
}

bool LogSegmentStore::SegmentExists(const SEGMENT_ID segment_id)
{
    const std::string file_path = ToSegmentPath(segment_id);
    if (access(file_path.c_str(), F_OK) == -1)
    {
        // segment not exists
        return false;
    }

    return true;
}

int LogSegmentStore::PaddingIfNewSegment()
{
    // seek to end
    off_t fileSize = lseek(cur_segment_fd_, 0, SEEK_END);
    if (fileSize == -1)
    {
        // on err, seek failed.
        return -1;
    }

    if (fileSize > 0)
    {
        // file not empty, no need padding
        cur_segment_file_size_ = fileSize;
        return 0;
    }

    // padding if file empty (recently created)
    // seek to last byte and write padding byte
    fileSize = lseek(cur_segment_fd_, segment_max_size_ - 1, SEEK_SET);
    if (fileSize != segment_max_size_ - 1)
    {
        // on err, padding length not match
        return -1;
    }

    ssize_t write_size = write(cur_segment_fd_, "\0", 1);
    if (write_size != 1)
    {
        // on err, write padding failed
        return -1;
    }

    fileSize = segment_max_size_;
    off_t offset = lseek(cur_segment_fd_, 0, SEEK_SET);
    if (offset != 0)
    {
        // on err
        return -1;
    }
    
    cur_segment_file_size_ = segment_max_size_;
    
    return 1;
}

bool LogSegmentStore::NewSegment(const size_t &sizeToWrite)
{
    bool st = UpdateMetadata();
    if (!st)
    {
        // on err, update metadata failed
        return false;
    }

    cur_segment_id_++;

    if(SegmentExists(cur_segment_id_))
    {
        // on err, new segment already exists, somehing chaos
        return false;
    }

    FD fd;
    st = OpenSegment(cur_segment_id_, fd);

    if (!st)
    {
        // on err, open segment file failed
        return false;
    }

    cur_segment_fd_ = fd;

    int padding_result = PaddingIfNewSegment();
    if (padding_result < 0)
    {
        close(cur_segment_fd_);
        // on err, padding failed
        return false;
    }

    cur_segment_offset_ = 0;

    return true;
}

bool LogSegmentStore::UpdateMetadata()
{
    // update metadata
    if(lseek(meta_fd_, 0, SEEK_SET) == -1)
    {
        // on err, seek metadata failed
        return false;
    }

    // on err, write new seg id
    SEGMENT_ID tmp_segid = cur_segment_id_ +1;
    size_t write_segid_len = write(meta_fd_, (char *)&tmp_segid, sizeof(SEGMENT_ID));
    if (write_segid_len != sizeof(SEGMENT_ID))
    {
        // on err, update metadata seg id failed
        return false;
    }

    // write new seg crc
    CHECKSUM new_seg_crc = crc32(0, (const Bytef *)(&tmp_segid), sizeof(SEGMENT_ID));
    size_t write_crc_len = write(meta_fd_, (char *)&new_seg_crc, sizeof(CHECKSUM));
    if (write_crc_len != sizeof(CHECKSUM))
    {
        // on err, update metadata seg crc failed
        return -1;
    }

    int ret = fsync(meta_fd_);
    if (ret != 0)
    {
        // on err, flush failed
        return false;
    }

    return true;
}

bool LogSegmentStore::OpenSegment(const SEGMENT_ID& segment_id, FD &fd, bool need_create)
{
    const std::string file_path = ToSegmentPath(segment_id);

    // create it if not exists and read&write permission for file owner
    
    fd = open(file_path.c_str(), O_RDWR | (need_create ? O_CREAT : O_RDONLY), S_IWUSR | S_IREAD);
    if (fd == -1)
    {
        // on err, open segment file failed
        return false;
    }

    return true;
}

const std::string LogSegmentStore::ToSegmentPath(const SEGMENT_ID &segment_id)
{
    std::string path = db_path_ + (db_path_.back() == '/' ? "" : "/") + std::to_string(segment_id);
    return path;
}

bool LogSegmentStore::DeleteSegment(const SEGMENT_ID segment_id)
{
    const std::string path = ToSegmentPath(segment_id);
    bool st = remove(path.c_str());
    if (st != 0)
    {
        // on err, delete segment failed.
        return false;
    }

    return true;
}

bool LogSegmentStore::DeleteSegmentBefore(const SEGMENT_ID segment_id)
{
    if (segment_id >= cur_segment_id_)
    {
        // on err, segment id not out of date
        return false;
    }
    
    auto to_delete_segment = segment_id;
    while (SegmentExists(to_delete_segment))
    {
        if (!DeleteSegment(to_delete_segment))
        {
            // on err, delete segment failed.
            return false;
        }

        to_delete_segment -= 1;
    }
    return true;
}

void LogSegmentStore::ToSegmentIndex(const SEGMENT_ID segment_id, const off_t offset, const CHECKSUM checksum, SegmentIndex &logIndex)
{
    LOG(INFO) << "segment_id:" << segment_id << ", offset:" << offset << ", checksum:" << checksum;
    size_t totalLen = sizeof(SEGMENT_ID) + sizeof(off_t) + sizeof(CHECKSUM);
    char concat[totalLen];
    memcpy(concat, &segment_id, sizeof(SEGMENT_ID));
    LOG(INFO) << "segment_id:" << segment_id;
    memcpy(concat + sizeof(SEGMENT_ID), &offset, sizeof(off_t));
    LOG(INFO) << "offset:" << offset;
    memcpy(concat + sizeof(SEGMENT_ID) + sizeof(off_t), &checksum, sizeof(CHECKSUM));
    LOG(INFO) << "checksum:" << checksum;

    logIndex = std::string(concat, totalLen);

    LOG(INFO) << "logIndex:" << logIndex << ", len:" << logIndex.size();
    
}

void LogSegmentStore::ParseSegmentIndex(const SegmentIndex &log_index, SEGMENT_ID &segment_id, off_t &offset, CHECKSUM &check_sum)
{
    memcpy(&segment_id, (void *)log_index.c_str(), sizeof(SEGMENT_ID));
    memcpy(&offset, (void *)(log_index.c_str() + sizeof(SEGMENT_ID)), sizeof(off_t));
    memcpy(&check_sum, (void *)(log_index.c_str() + sizeof(SEGMENT_ID) + sizeof(off_t)), sizeof(CHECKSUM));
}
