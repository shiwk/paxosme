/*
 * @Author: shiwk
 * @Date: 2023-06-25 23:01:00
 * @Last Modified by: shiwk
 * @Last Modified time: 2023-06-25 23:22:36
 */

#include "log_storage.hpp"

bool DataBaseLogStorage::Init(const LogStorageOptions &log_storage_options)
{
    if (!log_index_db_ || !log_index_db_->Init(log_storage_options))
    {
        return false;
    }

    if (!log_segment_store_ || !log_segment_store_->Init(log_storage_options))
    {
        return false;
    }

    if (!AlignIndexWithSegmentStore())
    {
        return false;
    }

    return true;
}

bool DataBaseLogStorage::Put(const LogEntryKey &key, const LogEntryValue &value)
{
    /*
    1. append segment (get offset) and log_entry_key to log index
    2. put log index
    */

    LogIndex log_index;
    bool has_log_index = GenerateLogIndex(key, value, log_index);

    if (!has_log_index)
    {
        return false;
    }

    IndexKey index_key = GenerateIndexKey(key);
    bool write_log_index = log_index_db_->PutLogIndex(index_key, log_index);
    if (!write_log_index)
    {
        return false;
    }

    return true;
}

bool DataBaseLogStorage::Get(const LogEntryKey &key, LogEntryValue &value)
{
    /*
    1. log_entry_key to log_index_key and get log index
    2. get log segment offset
    3. read file with offset
    4. return true if index_key == key_from_storage
    */

    // log_entry_key -> log_index_key
    IndexKey index_key = GenerateIndexKey(key);

    // get log index
    LogIndex log_index;
    bool res = log_index_db_->GetLogIndex(index_key, log_index);
    if (!res)
    {
        // get log inde failed
        return false;
    }

    // read segment with offset
    IndexKey key_from_segment;
    bool read_res = log_segment_store_->Read(log_index, key_from_segment, value);

    if (!read_res || key_from_segment != log_index)
    {
        // read failed
        return false;
    }

    return false;
}

IndexKey DataBaseLogStorage ::GenerateIndexKey(const LogEntryKey &log_entry_key)
{
    IndexKey index_key(log_entry_key);
    return index_key;
}

bool DataBaseLogStorage::GenerateLogIndex(const LogEntryKey &log_entry_key, const LogEntryValue &log_entry_value, LogIndex &log_index)
{
    bool append_res = log_segment_store_->Append(log_entry_key, log_entry_value, log_index);
    if (append_res)
    {
        return true;
    }

    return false;
}

bool DataBaseLogStorage::AlignIndexWithSegmentStore()
{
    // reload index in history in case failover (ie. failover between append to logsegment and write index to index_db)
    IndexKey last_index_key;

    bool last_index_key_exists = log_index_db_->GetLastIndexKey(last_index_key);

    //todo I: read segment stor from last_index_key
    

    return false;
}

paxosme::LogStorage *paxosme::LogStorage::New()
{
    auto logStorage = new DataBaseLogStorage;
    return logStorage;
}