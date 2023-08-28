//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_COMMON_H
#define PAXOSME_COMMON_H

#define NODE_DUMMY 0
#define PROPOSAL_DUMMY 0
#define INSTANCE_DUMMY 0

#include <cstdint>
#include <string>

using node_id_t = uint64_t;
using proposal_id_t = uint64_t;
using instance_id_t = uint64_t;
using index_t = uint64_t;
using LogValue = std::string;


class LongLife {
public:
    template <class TClass>
    static TClass* CreateInstance(){return TClass::New();}
};


#endif //PAXOSME_COMMON_H
