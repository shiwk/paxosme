
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

    bool Delete(const LogEntryKey &) override;

private:
    static SegmentIndex ToIndexKey(const LogEntryKey &);

private:
    bool AlignIndexWithSegmentStore();
    std::string dbpath_;
    LogIndexDB *log_index_db_;
    LogSegmentStore *segment_store_;
};
#endif
