
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

bool LogIndexDB::GetIndex(const std::string &idx_key, std::string &idx_val)
{
    leveldb::Status status = leveldb_->Get(leveldb::ReadOptions(), idx_key, &idx_val);
    return status.ok();
}

bool LogIndexDB::PutIndex(const std::string &idx_key, const std::string &idx_val)
{
    leveldb::Status status = leveldb_->Put(leveldb::WriteOptions(), idx_key, idx_val);
    return status.ok();
}

bool LogIndexDB::DelIndex(const std::string &idx_key)
{
    leveldb::Status status = leveldb_->Delete(leveldb::WriteOptions(), idx_key);
    return status.ok();
}

bool LogIndexDB::GetLastIndex(std::string &idx_val)
{
    auto iter = leveldb_->NewIterator(leveldb::ReadOptions());

    // iter is invalid before seek
    iter->SeekToLast();

    // assume that only logindex in this db instance, otherwise iter could be any other entry
    // todo II: maybe more cases to consider, such as invalid data
    if (!iter->Valid())
    {
        // no indexkey found
        idx_val = "";
        delete iter;
        return true;
    }
    delete iter;
    return GetIndex(iter->key().ToString(), idx_val);
}
