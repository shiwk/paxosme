//
// Created by kk on 2022/12/4.
//
#include "memory_db.hpp"

namespace paxosme {
    // todo I: impl
    void DirectMemory::Init() {}

    int DirectMemory::Put(const LogEntryKey llInstanceID, const LogEntryValue &sValue) {
//        safeMap_.Add(llInstanceID, sValue);
        safeMap_[llInstanceID] = sValue;
        return 1;
    }

    bool DirectMemory::Get(const LogEntryKey llInstanceID, LogEntryValue &sValue) {
        return safeMap_.Get(llInstanceID, sValue);
    }
}