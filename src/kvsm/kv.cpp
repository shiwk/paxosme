//
// Created by kk on 2023/5/31.
//
#include <kv.hpp>
#include <kv.pb.h>

bool KV::Init(const std::string &dbpath) {
    if (dbpath_.empty())
        return true;

    dbpath_ = dbpath;
    leveldb::Options oOptions;
    oOptions.create_if_missing = true;
    auto status = leveldb::DB::Open(oOptions, dbpath, &leveldb_);

    if (!status.ok()) {
        return false;
    }
    return true;
}

bool KV::Get(const std::string &key, std::string &value, version_id &ver) {
    if (dbpath_.empty())
        return false;

    std::string sBuffer;
    auto res = leveldb_->Get(leveldb::ReadOptions(), key, &sBuffer);
    if (!res.ok()) {
        // key not found
        return false;
    }

    paxosme::ValuePB value_pb;

    if (!value_pb.ParseFromArray(sBuffer.data(), sBuffer.size())) {
        // parsing error
        return false;
    }

    ver = value_pb.ver();

    return true;
}

bool KV::Put(const std::string &key, const std::string &value, version_id ver) {
    if (dbpath_.empty())
        return false;

    // lock for
    std::unique_lock<std::mutex> lock(mutex_);

    version_id ver_db;
    std::string v;
    Get(key, v, ver_db);

    if (ver_db != ver) {
        // version not match
        return false;
    }

    paxosme::ValuePB value_pb;
    value_pb.set_value(value);
    value_pb.set_ver(ver + 1);

    std::string serialized;
    bool serialization_result = value_pb.SerializeToString(&serialized);
    if (!serialization_result) {
        // serialization failed
        return false;
    }

    auto kv_status = leveldb_->Put(leveldb::WriteOptions(), key, serialized);

    if (!kv_status.ok()) {
        return false;
    }

    return true;
}

