/*
 * @Author: shiwk
 * @Date: 2023-06-25 23:01:00
 * @Last Modified by: shiwk
 * @Last Modified time: 2023-06-25 23:22:36
 */

#include "logstorage_db.hpp"

bool DataBaseLogStorage::Init(const LogStorageOptions &logStorageOptions)
{
    if (dbpath_.empty())
        return true;

    dbpath_ = logStorageOptions.dbPath;
    leveldb::Options oOptions;
    oOptions.create_if_missing = true;
    auto status = leveldb::DB::Open(oOptions, dbpath_, &leveldb_);

    if (!status.ok()) {
        return false;
    }
    return true;
}

int DataBaseLogStorage::Put(LogEntryKey llInstanceID, const LogEntryValue &sValue)
{
    // todo I: implement
    return 0;
}

bool DataBaseLogStorage::Get(LogEntryKey llInstanceID, LogEntryValue &sValue)
{
    return false;
}

paxosme::LogStorage* paxosme::LogStorage::New()
{
    auto logStorage = new DataBaseLogStorage;
    return logStorage;
}

