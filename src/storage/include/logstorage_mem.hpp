//
// Created by kk on 2023/2/5.
//


#include "logstorage.hpp"
#include <map.hpp>

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_MEM_HPP

#endif //PAXOSME_LOGSTORAGE_HPP

namespace paxosme {
    class MemLogStorage : public LogStorage {
    public:
        void Init() override;

        int Put(LogEntryId llInstanceID, const LogEntry &sValue) override;

        bool Get(LogEntryId llInstanceID, LogEntry &sValue) override;
    private:
        SafeMap<LogEntryId, LogEntry> safeMap_;
    };
}
