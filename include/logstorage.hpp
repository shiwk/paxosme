//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_HPP

#include <string>

using LogEntryValue = std::string;
using LogEntryKey = std::string;
#define LOG_ENTRY_KEY_LENGTH sizeof(uint64_t)

namespace paxosme
{
    class LogStorage
    {
    public:
        struct LogStorageOptions
        {
            std::string dbPath;
            size_t segmentMaxSize;
            size_t indexKeyLength = LOG_ENTRY_KEY_LENGTH;
        };
        static LogStorage *New();

        virtual bool Init(const LogStorageOptions &) = 0;

        virtual bool Put(const LogEntryKey &, const LogEntryValue &) = 0;

        virtual bool Get(const LogEntryKey &, LogEntryValue &) = 0;
    };

}
#endif // PAXOSME_LOGSTORAGE_HPP
