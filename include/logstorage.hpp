//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_HPP

#include <string>

using LogEntryValue = std::string;
using LogEntryKey = std::string;

namespace paxosme
{
    class LogStorage
    {
    public:
        struct LogStorageOptions
        {
            std::string dbPath;
        };
        static LogStorage *New();

        virtual bool Init(const LogStorageOptions &) = 0;

        virtual bool Put(const LogEntryKey &, const LogEntryValue &) = 0;

        virtual bool Get(const LogEntryKey &, LogEntryValue &) = 0;
    };

}
#endif // PAXOSME_LOGSTORAGE_HPP
