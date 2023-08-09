#ifndef INDEX_DB_HPP
#define INDEX_DB_HPP

#include <leveldb/db.h>
#include "logstorage.hpp"

using SegmentIndex = std::string;
using IndexKey = std::string;
// #define INDEX_KEY_LENGTH sizeof(uint64_t)

class LogIndexDB
{
public:
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool GetSegmentIndex(const IndexKey &, SegmentIndex &);
    bool PutSegmentIndex(const IndexKey &, const SegmentIndex &);
    static LogIndexDB *New();
    bool GetLastLogIndex(IndexKey &,SegmentIndex &);
private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
};

#endif
