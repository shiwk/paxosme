

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "logstorage.hpp"

class LogSegmentStore
{
public:
    bool Init(const LogStorageOptions &);
    bool Read(const LogIndex, IndexKey &, LogEntryValue &);
    static LogSegmentStore *New();
};

#endif
