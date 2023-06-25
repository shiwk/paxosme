#ifndef LOGSTORAGE_DB
#define LOGSTORAGE_DB

#include "logstorage.hpp"
#include <leveldb/db.h>

class DataBaseLogStorage : public paxosme::LogStorage
{
public:
    bool Init(const LogStorageOptions &) override;

    int Put(LogEntryKey llInstanceID, const LogEntryValue &sValue) override;

    bool Get(LogEntryKey llInstanceID, LogEntryValue &sValue) override;

private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
};

#endif