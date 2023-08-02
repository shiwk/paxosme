

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "logstorage.hpp"
#include <mutex>

#define SEGMENT_STORE_DIR "/segment"
#define METADATA_FILE "/.metadata"

using SEGMENT_ID = uint32_t;
// using SEGMENT_ID = int;
using CHECKSUM = unsigned long;
class LogSegmentStore
{
public:
    using FD = int;

    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool Read(const SegmentLogIndex, IndexKey &, LogEntryValue &);
    bool Append(const LogEntryKey &, const LogEntryValue &, SegmentLogIndex &);
    SEGMENT_ID GetLastSegementId();
    bool Replay(const SEGMENT_ID &, off_t &, IndexKey &, SegmentLogIndex &);

    static LogSegmentStore *New();

    // static void ParseLogIndex(const LogIndex &, SEGMENT_ID &, off_t &, CHECKSUM &);
    static void ToSegmentLogIndex(const SEGMENT_ID, const off_t, const CHECKSUM, SegmentLogIndex &);

private:
    static bool PathExistsOrCreate(const std::string &);
    static bool PathExists(const std::string &);
    static int PaddingIfNewFile(const FD, size_t &fileSize, size_t padding_length);

private:
    std::string db_path_;
    FD meta_fd_;
    std::mutex mutex_;
    SEGMENT_ID cur_segment_id_;
    FD cur_segment_fd_;
    off_t cur_segment_offset_;
    size_t cur_segment_file_size_;
    size_t index_key_length_;
};

#endif
