#include "segment_store.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include <zconf.h>

bool LogSegmentStore::Init(const paxosme::LogStorage::LogStorageOptions &options)
{
    if (!db_path_.empty())
    {
        return false;
    }

    db_path_ = options.dbPath + SEGMENT_STORE_DIR;
    index_key_length_ = options.indeKeyLength;

    if (!PathExistsOrCreate(db_path_))
    {
        return false;
    }

    const std::string &path = db_path_ + METADATA_FILE;
    auto meta_data_cpath = path.c_str();

    meta_fd_ = open(meta_data_cpath,
                    O_CREAT | O_RDWR /* creat if not exists and open for read&write */,
                    S_IREAD | S_IWRITE /* Read, Write by owner */
    );

    if (meta_fd_ == -1)
    {
        // open meta data file failed
        return false;
    }

    // lseek meta file to 0
    off_t head_pos = lseek(meta_fd_, 0, SEEK_SET);

    if (head_pos == -1)
    {
        // lseek failed
        return false;
    }

    // read first SEGMENT_ID as segment id
    ssize_t read_len = read(meta_fd_, &cur_segment_id_, sizeof(SEGMENT_ID));
    if (read_len == 0)
    {
        // means first time init if read SEGMENT_ID empty, set 0 to cur_segment_id_
        cur_segment_id_ = 0;
    }
    else if (read_len != (ssize_t)sizeof(SEGMENT_ID))
    {
        // read SEGMENT_ID failed
        return false;
    }

    // read next crc and check equality with calculation result
    uint32_t read_check_sum = 0;
    read_len = read(meta_fd_, &read_check_sum, sizeof(CHECKSUM));
    if (read_len == (ssize_t)sizeof(CHECKSUM))
    {
        CHECKSUM cal_check_sum = crc32(0, (const Bytef *)(&cur_segment_id_), sizeof(SEGMENT_ID));

        if (cal_check_sum != read_check_sum)
        {
            // checksum not match
            return false;
        }
    }

    const std::string file_path = db_path_ + "/" + std::to_string(cur_segment_id_);

    // create it if not exists and read&write permission for file owner
    cur_segment_fd_ = open(file_path.c_str(), O_CREAT | O_RDWR, S_IWUSR | S_IREAD);
    if (cur_segment_fd_ == -1)
    {
        // open segment file failed
        return false;
    }

    // padding
    int padding_result = PaddingIfNewFile(cur_segment_fd_, cur_segment_file_size_, options.segmentMaxSize);
    if (padding_result < 0)
    {
        // on error
        return false;
    }

    if (padding_result == 1)
    {
        // seek to begin
        int offset = lseek(cur_segment_fd_, 0, SEEK_SET);
        cur_segment_offset_ = 0;
        if (offset != 0)
        {
            // on error, seek failed
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

bool LogSegmentStore::Append(const LogEntryKey &, const LogEntryValue &, SegmentIndex &)
{
    // todo I: segment append.
    return false;
}

bool LogSegmentStore::ReplaySegment(const SEGMENT_ID &segment_id, off_t &offset)
{
    IndexKey next_index_key;
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

bool LogSegmentStore::ReplayLog(const SEGMENT_ID &segment_id, off_t &offset, IndexKey &index_key, SegmentIndex &log_index)
{
    const std::string file_path = db_path_ + "/" + std::to_string(segment_id);

    if (access(file_path.c_str(), F_OK) == -1)
    {
        // segment not exists
        offset = -1;
        return false;
    }

    FD fd = open(file_path.c_str(),
                 O_RDWR /* creat if not exists and open for read&write */,
                 S_IREAD | S_IWRITE /* Read, Write by owner */
    );

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

    int length = 0;
    // read first length
    ssize_t read_len = read(fd, (char *)&length, sizeof(int));
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

bool LogSegmentStore::PathExistsOrCreate(const std::string &path)
{
    struct stat path_info;
    const char *cpath = path.c_str();
    if (stat(cpath, &path_info) != 0)
    {
        // not exixts and create
        if (mkdir(cpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH /*Read, Write, Execute by owner and others*/) == -1)
        {
            // on error, create dir failed
            return false;
        }
    }
    else if (!(path_info.st_mode & S_IFDIR))
    {
        // on error, exists but not directory
        return false;
    }
    return true;
}

int LogSegmentStore::PaddingIfNewFile(const FD fd, size_t &fileSize, size_t padding_length)
{
    // seek to end
    fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1)
    {
        // on error, seek failed.
        return -1;
    }

    if (fileSize > 0)
    {
        // file not empty, no need padding
        return 0;
    }

    // padding if file empty (recently created)
    // seek to last byte and write padding byte
    fileSize = lseek(fd, padding_length - 1, SEEK_SET);
    if (fileSize != padding_length - 1)
    {
        // on error, padding length not match
        return -1;
    }

    ssize_t write_size = write(fd, "\0", 1);
    if (write_size != 1)
    {
        // on error, write padding failed
        return -1;
    }

    fileSize = padding_length;
    return 1;
}


void LogSegmentStore::ToSegmentIndex(const SEGMENT_ID segment_id, const off_t offset, const CHECKSUM checksum, SegmentIndex &logIndex)
{
    logIndex = std::string(sizeof(int) + sizeof(int) + sizeof(uint32_t), 0);
    memcpy((void *)logIndex.c_str(), (char *)&segment_id, sizeof(SEGMENT_ID));
    memcpy((void *)(logIndex.c_str() + sizeof(SEGMENT_ID)), (char *)&offset, sizeof(off_t));
    memcpy((void *)(logIndex.c_str() + sizeof(SEGMENT_ID) + sizeof(off_t)), (char *)&checksum, sizeof(CHECKSUM));
}
