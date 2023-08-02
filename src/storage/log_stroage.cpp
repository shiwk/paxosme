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

    SegmentLogIndex log_index;
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
    SegmentLogIndex log_index;
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

bool DataBaseLogStorage::GenerateLogIndex(const LogEntryKey &log_entry_key, const LogEntryValue &log_entry_value, SegmentLogIndex &log_index)
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
    SegmentLogIndex exist_log_index;
    IndexKey exist_index_key;
    bool last_index_key_exists = log_index_db_->GetLastLogIndex(exist_index_key, exist_log_index);

    if (!last_index_key_exists)
    {
        // no index in indexstore, directly return
        // todo II: should consider the case that log exists in the first segment, but not stored in index db
        return true;
    }

    SEGMENT_ID db_segment_id;
    off_t segment_offset;
    CHECKSUM checksum;
    ParseLogIndex(exist_log_index, db_segment_id, segment_offset, checksum);
    SEGMENT_ID last_segement_id = log_segment_store_->GetLastSegementId();

    if (last_segement_id < db_segment_id)
    {
        // some chaos happened, log in index_db should not be higher than that in segement store
        return false;
    }

    auto segment_id = db_segment_id;
    while (true)
    {
        // todo I: replay next segement from segmentstore, and put into indexstore if any exists
        //  replay offset should always start from zero except the first time, as offset exists for the last log in index_db
        SegmentLogIndex next_log_index;
        IndexKey next_index_key;
        while (log_segment_store_->Replay(segment_id, segment_offset, next_index_key, next_log_index))
        {
            log_index_db_->PutLogIndex(next_index_key, next_log_index);
        }

        if (segment_offset > 0)
        {
            // reach end of this segment
            segment_offset = 0;
            segment_id += 1;
            continue;
        }
        else 
        {
            // segment not exists 
            break;
        }
    }

    // todo I: how to pos "now_offset" of segment store for next append()

    return true;
}

paxosme::LogStorage *paxosme::LogStorage::New()
{
    auto logStorage = new DataBaseLogStorage;
    return logStorage;
}

void DataBaseLogStorage::ParseLogIndex(const SegmentLogIndex & log_index, SEGMENT_ID &segment_id, off_t & offset, CHECKSUM &check_sum)
{
    memcpy(&segment_id, (void *)log_index.c_str(), sizeof(SEGMENT_ID));
    memcpy(&offset, (void *)(log_index.c_str() + sizeof(SEGMENT_ID)), sizeof(off_t));
    memcpy(&check_sum, (void *)(log_index.c_str() + sizeof(SEGMENT_ID) + sizeof(off_t)), sizeof(uint32_t));
}