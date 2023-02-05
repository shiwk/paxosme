//
// Created by kk on 2022/12/4.
//
#include "logstorage_mem.hpp"

namespace paxosme {
    // todo I: impl
    void MemLogStorage::Init() {}

    int MemLogStorage::Put(const LogEntryId llInstanceID, const LogEntry &sValue) {
//        safeMap_.Add(llInstanceID, sValue);
        safeMap_[llInstanceID] = sValue;
        return 1;
    }

    bool MemLogStorage::Get(const LogEntryId llInstanceID, LogEntry &sValue) {
        return safeMap_.Get(llInstanceID, sValue);
    }
}