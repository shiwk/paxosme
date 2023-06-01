//
// Created by kk on 2023/5/31.
//

#ifndef PAXOSME_KV_HPP
#define PAXOSME_KV_HPP

#include <leveldb/db.h>

using version_id = uint64_t;

class KV {
public:
    bool Init(const std::string &dbpath);

    bool Get(const std::string &key, std::string &value, version_id &);

    bool Put(const std::string &key, const std::string &value);

private:
    leveldb::DB *leveldb_;
    std::string dbpath_;
};

#endif //PAXOSME_KV_HPP
