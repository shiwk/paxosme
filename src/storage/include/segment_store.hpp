

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "logstorage.hpp"
#include <mutex>

#define SEGMENT_STORE_DIR "/segment"
#define METADATA_FILE "/.metadata"

class LogSegmentStore
{
    using FD = int;
    using FID = uint16_t;
    using SEGMENTID = int;

public:
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool Read(const LogIndex, IndexKey &, LogEntryValue &);
    bool Append(const LogEntryKey &, const LogEntryValue &, LogIndex &);
    static LogSegmentStore *New();

private:
    bool PathExistsOrCreate(const std::string &);
private:
    std::string db_path_;
    FD meta_fd_;
    std::mutex mutex_;
    SEGMENTID cur_segment_id_;
};

#endif
