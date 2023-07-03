
#include "index_db.hpp"

bool LogIndexDB::Init(const paxosme::LogStorage::LogStorageOptions &log_storage_options)
{
    if (dbpath_.empty())
        return true;

    dbpath_ = log_storage_options.dbPath;
    leveldb::Options oOptions;
    oOptions.create_if_missing = true;
    auto status = leveldb::DB::Open(oOptions, dbpath_, &leveldb_);

    if (!status.ok())
    {
        return false;
    }
    return true;
}