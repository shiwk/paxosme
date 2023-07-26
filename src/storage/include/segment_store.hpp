

#ifndef SEGMENT_STORE_HPP
#define SEGMENT_STORE_HPP

#include "index_db.hpp"
#include "logstorage.hpp"
#include <mutex>

#define SEGMENT_STORE_DIR "/segment"
#define METADATA_FILE "/.metadata"

class LogSegmentStore
{
public:
    using FD = int;
    using SEGMENT_ID = uint16_t;
    // using SEGMENT_ID = int;
    using CHECKSUM = unsigned long; 
    
    bool Init(const paxosme::LogStorage::LogStorageOptions &);
    bool Read(const LogIndex, IndexKey &, LogEntryValue &);
    bool Append(const LogEntryKey &, const LogEntryValue &, LogIndex &);
    SEGMENT_ID GetLastSegementId();
    static LogSegmentStore *New();

    static void ParseLogIndex(const LogIndex &, SEGMENT_ID &, off_t &, CHECKSUM &);
    static void ToLogIndex(const SEGMENT_ID,const off_t,const CHECKSUM, LogIndex &);

private:
    bool PathExistsOrCreate(const std::string &);
    static int PaddingIfNewFile(const FD, size_t &fileSize, size_t padding_length);

private:
    std::string db_path_;
    FD meta_fd_;
    std::mutex mutex_;
    SEGMENT_ID cur_segment_id_;
    FD cur_segment_fd_;
    off_t cur_segment_offset_;
    size_t cur_segment_file_size_;
};

#endif
