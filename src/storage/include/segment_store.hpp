

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "logstorage.hpp"

class LogSegmentStore
{
public:
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool Read(const LogIndex, IndexKey &, LogEntryValue &);
    bool Append(const LogEntryKey &, const LogEntryValue &, LogIndex &);
    static LogSegmentStore *New();
};

#endif
