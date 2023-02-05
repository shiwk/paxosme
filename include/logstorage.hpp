//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_HPP

#include <string>

using LogEntry = std::string;
using LogEntryId = uint64_t;

namespace paxosme {
    class LogStorage {
    public:
        virtual void Init()=0;

        virtual int Put(LogEntryId llInstanceID, const LogEntry &sValue) = 0;

        virtual bool Get(LogEntryId llInstanceID, LogEntry &sValue) = 0;
    };

}
#endif //PAXOSME_LOGSTORAGE_HPP
