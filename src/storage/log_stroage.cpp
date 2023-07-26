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
    LogIndex log_index;

    bool last_index_key_exists = log_index_db_->GetLastLogIndex(log_index);

    if (!last_index_key_exists)
    {
        // no index in indexstore, directly return
        return true;
    }

    
    LogSegmentStore::SEGMENT_ID db_segement_id;
    off_t offset;
    LogSegmentStore::CHECKSUM checksum;
    LogSegmentStore::ParseLogIndex(log_index, db_segement_id, offset, checksum);
    LogSegmentStore::SEGMENT_ID last_segement_id = log_segment_store_->GetLastSegementId();

    if (last_segement_id < db_segement_id) 
    {
        // some chaos happened, log in index_db should not be higher than that in segement store
        return false;
    }
    
    
    auto segement_id = db_segement_id;
    while (true)
    {   
        //todo I: replay next segement from segmentstore, and put into indexstore if any exists 
        // replay offset should always start from zero except the first time, as offset exists for the last log in index_db
    }
    
    
    return false;
}

paxosme::LogStorage *paxosme::LogStorage::New()
{
    auto logStorage = new DataBaseLogStorage;
    return logStorage;
}