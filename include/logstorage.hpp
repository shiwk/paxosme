//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_HPP

#include <string>

namespace paxosme
{
    class LogStorage
    {
    public:
        struct LogStorageOptions
        {
        };

        using LogEntryValue = std::string;
        using LogEntryKey = std::string;

        virtual void Init() = 0;

        virtual int Put(LogEntryKey log_entry_id, const LogEntryValue &) = 0;

        virtual bool Get(LogEntryKey llInstanceID, LogEntryValue &sValue) = 0;
    };

}
#endif // PAXOSME_LOGSTORAGE_HPP
