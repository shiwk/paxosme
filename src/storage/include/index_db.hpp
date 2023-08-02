#ifndef INDEX_DB_HPP
#define INDEX_DB_HPP

#include <leveldb/db.h>
#include "logstorage.hpp"

using SegmentLogIndex = std::string;
using IndexKey = std::string;
#define INDEX_KEY_LENGTH sizeof(uint64_t)

class LogIndexDB
{
public:
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool GetLogIndex(const IndexKey &, SegmentLogIndex &);
    bool PutLogIndex(const IndexKey &, const SegmentLogIndex &);
    static LogIndexDB *New();
    bool GetLastLogIndex(IndexKey &,SegmentLogIndex &);
private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
};

#endif
