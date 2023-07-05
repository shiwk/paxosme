#include "segment_store.hpp"
#include <sys/stat.h>
#include <fcntl.h>

bool LogSegmentStore::Init(const paxosme::LogStorage::LogStorageOptions &options)
{
    if (!path_.empty())
    {
        return false;
    }

    // todo I: implement
    /*
    1. read metafile
        1.1 compact meta file path
        1.2 open meta file and get metafile fd
    */
    path_ = options.dbPath + SEGMENT_STORE_DIR;

    if (!PathExistsOrCreate(path_))
    {
        return false;
    }

    auto meta_data_cpath = (path_ + METADATA_FILE).c_str();

    meta_data_fd_ = open(meta_data_cpath,
                         O_CREAT | O_RDWR /* creat if not exists and open for read&write */,
                         S_IREAD | S_IWRITE /* Read, Write by owner */
    );

    if (meta_data_fd_ == -1)
    {
        // open meta data file failed
        return false;
    }
    
    /*
        1.3 lseek meta file to 0 and ead first fid
            1.3.1 means first time init, if empty, set 0 to writing-file-id
        1.4 read next crc and check equality with calculation result
    */

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
