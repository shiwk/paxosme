

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
// #include "logstorage.hpp"
#include <mutex>

#define SEGMENT_STORE_DIR "/segment"
#define METADATA_FILE "/.metadata"

using SEGMENT_ID = int32_t;
using CHECKSUM = unsigned long;
using SegmentIndex = std::string;

class LogSegmentStore
{
public:
    using FD = int;

    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool Read(const SegmentIndex & segment_index, std::string &key_in_segment, std::string &value_in_segment);
    bool Append(const std::string &, const std::string &, SegmentIndex &);
    SEGMENT_ID GetLastSegmentId();
    bool ReplaySegment(const SEGMENT_ID &, off_t &);
    static LogSegmentStore *New();

    // static void ParseLogIndex(const LogIndex &, SEGMENT_ID &, off_t &, CHECKSUM &);
    static void ToSegmentIndex(const SEGMENT_ID, const off_t, const CHECKSUM, SegmentIndex &);
    bool ReplayLog(const SEGMENT_ID &, off_t &, SegmentIndex &, SegmentIndex &);
    static void ParseLogIndex(const SegmentIndex &, SEGMENT_ID &, off_t &, CHECKSUM &);

private:
    static bool DirExistsOrCreate(const std::string &);
    static bool PathExists(const std::string &);
    int PaddingIfNewSegment();
    bool NewSegment(const size_t &toWriteSize);
    bool UpdateMetadata();
    bool OpenSegment(const SEGMENT_ID&, FD &);
    const std::string ToSegmentPath(const SEGMENT_ID&);

private:
    std::string db_path_;
    FD meta_fd_;
    std::mutex mutex_;
    SEGMENT_ID cur_segment_id_ = -1;
    FD cur_segment_fd_ = -1;
    off_t cur_segment_offset_ = -1;
    size_t cur_segment_file_size_;
    size_t index_key_length_;
    size_t segment_max_size_;
};

#endif
