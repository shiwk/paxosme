//
// Created by kk on 2023/2/5.
//


#include "logstorage.hpp"
#include <map.hpp>

#ifndef PAXOSME_LOGSTORAGE_HPP
#define PAXOSME_LOGSTORAGE_MEM_HPP

#endif //PAXOSME_LOGSTORAGE_HPP

namespace paxosme {
    class DirectMemory : public LogStorage {
    public:
        void Init() override;

        int Put(LogEntryKey llInstanceID, const LogEntryValue &sValue) override;

        bool Get(LogEntryKey llInstanceID, LogEntryValue &sValue) override;
    private:
        SafeMap<LogEntryKey, LogEntryValue> safeMap_;
    };
}
