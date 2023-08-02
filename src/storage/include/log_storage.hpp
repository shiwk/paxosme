
#ifndef LOG_STORAGE_HPP
#define LOG_STORAGE_HPP

#include "logstorage.hpp"
#include "common.hpp"
#include "index_db.hpp"
#include "segment_store.hpp"

class DataBaseLogStorage : public paxosme::LogStorage
{
public:
    bool Init(const LogStorageOptions &) override;

    bool Put(const LogEntryKey &, const LogEntryValue &) override;

    bool Get(const LogEntryKey &, LogEntryValue &) override;

    static void ParseLogIndex(const SegmentLogIndex &, SEGMENT_ID &, off_t &, CHECKSUM &);
    static IndexKey GenerateIndexKey(const LogEntryKey &);

private:
    bool GenerateLogIndex(const LogEntryKey &, const LogEntryValue &, SegmentLogIndex &);
    bool AlignIndexWithSegmentStore();
    std::string dbpath_;
    LogIndexDB *log_index_db_;
    LogSegmentStore *log_segment_store_;
};
#endif
