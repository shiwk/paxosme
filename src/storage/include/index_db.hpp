#ifndef INDEX_DB_HPP
#define INDEX_DB_HPP

#include <leveldb/db.h>
#include "logstorage.hpp"

class LogIndexDB
{
public:
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool GetIndex(const std::string &idx_key, std::string &idx_val);
    bool PutIndex(const std::string &idx_key, const std::string &idx_val);
    bool DelIndex(const std::string &idx_key);
    static LogIndexDB *New();
    bool GetLastIndex(std::string &);
private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
};

#endif
