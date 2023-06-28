
#ifndef LOG_STORAGE_HPP
#define LOG_STORAGE_HPP

#include "logstorage.hpp"
#include "common.hpp"
#include <leveldb/db.h>
#include "index_db.hpp"
#include "segment_store.hpp"

class DataBaseLogStorage : public paxosme::LogStorage
{
public:
    bool Init(const LogStorageOptions &) override;

    int Put(const LogEntryKey &, const LogEntryValue &) override;

    bool Get(const LogEntryKey &, LogEntryValue &) override;

    static IndexKey GenerateIndexKey(const LogEntryKey);

private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
    LogIndexDB *log_index_db_;
    LogSegmentStore *log_segment_store_;
};
#endif
