#include "segment_store.hpp"

bool LogSegmentStore::Init(const paxosme::LogStorage::LogStorageOptions &options)
{
    // todo I: implement
    /*
    1. read metafile
        1.1 compact meta file path
        1.2 open meta file and get metafile fd and lseek meta file to 0 
        1.3 read first fid
            1.3.1 means first time init, if empty, set 0 to writing-file-id
        1.4 read next crc and check equality with calculation result
    2. reload index in history in case failover (ie. failover between append to logsegment and write index to index_db)
    3. open the last file (create new if first time init) and locate offset

    */
    return false;
}
