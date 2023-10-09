

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "common.hpp"
// #include "logstorage.hpp"
#include <mutex>
#include <schedule.hpp>
#include <future>

#define SEGMENT_STORE_DIR "segment"
#define METADATA_FILE ".metadata"
#define SEGMENT_CLEAN_DELAY_IN_SECONDS 3

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
    bool RemoveAsync(const SegmentIndex &);
    int ReplayLog(const SEGMENT_ID &, const off_t &, std::string &index_key, SegmentIndex &);
    SEGMENT_ID GetLastSegmentId();
    off_t GetCurrentOffset();
    
    static LogSegmentStore *New();
    static LogSegmentStore *SingleInstance();
    static void ToSegmentIndex(const SEGMENT_ID, const off_t, const CHECKSUM, SegmentIndex &);
    static void ParseSegmentIndex(const SegmentIndex &, SEGMENT_ID &, off_t &, CHECKSUM &);
    ~LogSegmentStore();

private:
    static bool CreateIfNotExists(const std::string &);
    bool SegmentExists(const SEGMENT_ID);
    int PaddingIfNewSegment();
    bool NewSegment(const size_t &toWriteSize);
    bool UpdateMetadata();
    bool OpenSegment(const SEGMENT_ID&, FD &, bool = true);
    const std::string ToSegmentPath(const SEGMENT_ID&);
    bool DeleteSegment(const SEGMENT_ID);
    bool DeleteSegmentBefore(const SEGMENT_ID);
    bool ReplaySegment(const SEGMENT_ID &, off_t &);

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
    std::shared_ptr<Scheduler> scheduler_ = ShortLife::CreateInstance<Scheduler>();
    std::future<void *> segment_clean_future_;
};

#endif
