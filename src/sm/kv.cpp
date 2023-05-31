//
// Created by kk on 2023/5/31.
//
#include "kv.hpp"

bool KV::Init(const std::string &) {
    return false;
}

KV *KV::MYKV() {
    static KV my_kv;
    return &my_kv;
}

bool KV::Get(const std::string &key, std::string &value, version_id &) {
    return false;
}

bool KV::Put(const std::string &key, const std::string &value) {
    return false;
}

