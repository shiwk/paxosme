/*
 * @Author: shiwk
 * @Date: 2023-06-25 23:01:00
 * @Last Modified by: shiwk
 * @Last Modified time: 2023-06-25 23:22:36
 */

#include "log_storage.hpp"
#include <glog/logging.h>

DataBaseLogStorage::DataBaseLogStorage(std::shared_ptr<LogIndexDB> logIndexDb, std::shared_ptr<LogSegmentStore> logSegmentsStore)
    : log_index_db_(logIndexDb), segment_store_(logSegmentsStore)
{
}

bool DataBaseLogStorage::Init(const LogStorageOptions &log_storage_options)
{
    if (!log_index_db_ || !log_index_db_->Init(log_storage_options))
    {
        return false;
    }

    if (!segment_store_ || !segment_store_->Init(log_storage_options))
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

    SegmentIndex log_index;
    bool has_log_index = segment_store_->Append(key, value, log_index);

    if (!has_log_index)
    {
        return false;
    }

    std::string index_key = ToIndexKey(key);
    bool write_log_index = log_index_db_->PutIndex(index_key, log_index);
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
    std::string index_key = ToIndexKey(key);

    // get log index
    SegmentIndex segment_index;
    bool res = log_index_db_->GetIndex(index_key, segment_index);
    if (!res)
    {
        // on err, get log inde failed
        return false;
    }

    // read segment with offset
    LOG(INFO) << "segment index: " << segment_index << " key: " << *((instance_id_t*)key.c_str()) << " value: " << value;
    std::string key_in_segment;
    bool read_res = segment_store_->Read(segment_index, key_in_segment, value);

    if (!read_res)
    {
        LOG(ERROR) << "read segment failed";
        return false;
    }

    if (key_in_segment != index_key)
    {
        // on err, key not matched
        LOG(ERROR) << "key not matched";
        return false;
    }

    return true;
}

bool DataBaseLogStorage::Delete(const LogEntryKey &log_entry_key)
{
    std::string idx_key = ToIndexKey(log_entry_key);
    SegmentIndex segment_index;
    bool st = log_index_db_->GetIndex(idx_key, segment_index);

    if (!st)
    {
        // on err, index not exists
        return false;
    }

    st = segment_store_->RemoveAsync(segment_index);

    if (!st)
    {
        // on err, segment remove failed
        return false;
    }

    st = log_index_db_->DelIndex(idx_key);
    if (!st)
    {
        // on err, index delete failed
        return false;
    }

    return true;
}

DataBaseLogStorage *DataBaseLogStorage::New(std::shared_ptr<LogIndexDB> log_index_db, std::shared_ptr<LogSegmentStore>log_segment_store)
{
    return new DataBaseLogStorage(log_index_db, log_segment_store);
}

std::string DataBaseLogStorage ::ToIndexKey(const LogEntryKey &log_entry_key)
{
    // todo II: TBD
    return log_entry_key;
}

bool DataBaseLogStorage::AlignIndexWithSegmentStore()
{
    // reload index in history in case failover (ie. failover between append to logsegment and write index to index_db)
    SegmentIndex exist_log_index;
    std::string idx_key;
    bool last_index_key_exists = log_index_db_->GetLastIndex(idx_key, exist_log_index);

    if (!last_index_key_exists)
    {
        // no index in indexstore, directly return
        // todo II: should consider the case that log exists in the first segment, but not stored in index db
        return true;
    }

    SEGMENT_ID db_segment_id;
    off_t segment_offset;
    CHECKSUM checksum;
    LogSegmentStore::ParseSegmentIndex(exist_log_index, db_segment_id, segment_offset, checksum);
    SEGMENT_ID last_segment_id = segment_store_->GetLastSegmentId();

    if (last_segment_id < db_segment_id)
    {
        // some chaos happened, log in index_db should not be higher than that in segment store
        return false;
    }

    SegmentIndex next_log_index;
    std::string next_index_key;
    SEGMENT_ID segment_id = db_segment_id;
    
    while (true)
    {
        segment_offset = segment_store_->ReplayLog(segment_id, segment_offset, next_index_key, next_log_index);
        if (segment_offset == 0)
        {
            // reach end of this segment
            segment_offset = 0;
            segment_id += 1;
            continue;
        }

        if (segment_offset < 0)
        {
            // segment not exists
            break;
        }

        log_index_db_->PutIndex(next_index_key, next_log_index);
    }

    return true;
}


