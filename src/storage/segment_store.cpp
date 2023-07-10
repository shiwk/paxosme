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

    // todo I: implement
    /*
    1. read metafile
        1.1 compact meta file path
        1.2 open meta file and get metafile fd
    */
    db_path_ = options.dbPath + SEGMENT_STORE_DIR;

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

    // read first SEGMENTID as segment id
    ssize_t read_len = read(meta_fd_, &cur_segment_id_, sizeof(SEGMENTID));
    if (read_len == 0)
    {
        // means first time init if read SEGMENTID empty, set 0 to cur_segment_id_
        cur_segment_id_ = 0;
    }
    else if (read_len != (ssize_t)sizeof(SEGMENTID))
    {
        // read SEGMENTID failed
        return false;
    }

    // read next crc and check equality with calculation result
    uint32_t read_check_sum = 0;
    read_len = read(meta_fd_, &read_check_sum, sizeof(uLong));
    if (read_len == (ssize_t)sizeof(uLong))
    {
        uLong cal_check_sum = crc32(0, (const Bytef *)(&cur_segment_id_), sizeof(SEGMENTID));

        if (cal_check_sum != read_check_sum)
        {
            // checksum not match
            return false;
        }
    }

    /*
     2. reload index in history in case failover (ie. failover between append to logsegment and write index to index_db)
     3. open the last file (create new if first time init) and locate offset
    */

    return false;
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
            // create dir failed
            return false;
        }
    }
    else if (!(path_info.st_mode & S_IFDIR))
    {
        // exists but not directory
        return false;
    }
    return true;
}
