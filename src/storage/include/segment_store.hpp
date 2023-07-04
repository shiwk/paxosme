

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "logstorage.hpp"
#include <mutex>

class LogSegmentStore
{
    using FD = int64_t;
    using FID = uint16_t;

public:
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool Read(const LogIndex, IndexKey &, LogEntryValue &);
    bool Append(const LogEntryKey &, const LogEntryValue &, LogIndex &);
    static LogSegmentStore *New();

private:
    private : std::string path_;
    std::mutex mutex_;
};

#endif
