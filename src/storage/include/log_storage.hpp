
#ifndef LOG_STORAGE_HPP
#define LOG_STORAGE_HPP

#include "logstorage.hpp"
#include "common.hpp"
#include "index_db.hpp"
#include "segment_store.hpp"

class DataBaseLogStorage : public paxosme::LogStorage
{
public:
    DataBaseLogStorage(std::shared_ptr<LogIndexDB>, std::shared_ptr<LogSegmentStore>);
    bool Init(const LogStorageOptions &) override;

    bool Put(const LogEntryKey &, const LogEntryValue &) override;

    bool Get(const LogEntryKey &, LogEntryValue &) override;

    bool Delete(const LogEntryKey &) override;

    static DataBaseLogStorage* New(std::shared_ptr<LogIndexDB>, std::shared_ptr<LogSegmentStore>);

private:
    static SegmentIndex ToIndexKey(const LogEntryKey &);

private:
    bool AlignIndexWithSegmentStore();
    std::string dbpath_;
    std::shared_ptr<LogIndexDB> log_index_db_;
    std::shared_ptr<LogSegmentStore> segment_store_;
};
#endif
