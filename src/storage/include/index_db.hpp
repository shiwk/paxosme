#ifndef INDEX_DB_HPP
#define INDEX_DB_HPP

#include <leveldb/db.h>
#include "logstorage.hpp"

using LogIndex = std::string;
using IndexKey = std::string;

class LogIndexDB
{
public:
    bool Init(const LogStorageOptions &);
    bool GetLogIndex(const IndexKey, LogIndex &);
    bool PutLogIndex(const IndexKey, const LogIndex &);
    static LogIndexDB *New();

private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
};

#endif
