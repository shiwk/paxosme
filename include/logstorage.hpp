//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_HPP

#include <string>

using LogEntry = std::string;
using LogEntryId = uint64_t;

namespace paxosme{
    class LogStorage {
    public:
        void Init();
        bool Write(const LogEntryId llInstanceID, const LogEntry & sValue);
        bool Read(const LogEntryId llInstanceID, const LogEntry & sValue);
    };

}
#endif //PAXOSME_LOGSTORAGE_HPP
