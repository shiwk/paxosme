
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

bool LogIndexDB::GetSegmentIndex(const IndexKey & index_key, SegmentIndex &log_index)
{
    leveldb::Status status = leveldb_->Get(leveldb::ReadOptions(), index_key, &log_index);
    return status.ok();
}

bool LogIndexDB::PutSegmentIndex(const IndexKey &index_key, const SegmentIndex &log_index)
{
    leveldb::Status status = leveldb_->Put(leveldb::WriteOptions(), index_key, log_index);
    return status.ok();
}

bool LogIndexDB::GetLastLogIndex(IndexKey &index_key, SegmentIndex & log_index)
{
    auto iter = leveldb_->NewIterator(leveldb::ReadOptions());
    
    // iter is invalid before seek
    iter->SeekToLast();

    // assume that only logindex in this db instance, otherwise iter could be any other entry 
    // todo II: maybe more cases to consider, such as invalid data
    if (!iter->Valid())
    {
        // no indexkey found
        log_index = "";
        delete iter;
        return true;
    }
    delete iter;
    return GetSegmentIndex(iter->key().ToString(),  log_index);
}
