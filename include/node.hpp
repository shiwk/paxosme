//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_NODE_HPP
#define PAXOSME_NODE_HPP

#include <string>
#include <vector>
#include "config.hpp"

namespace paxosme {
    struct NodeIdList {
        std::vector<node_id_t> idList;
    };

    class Node {
    public:
        virtual ~Node() = 0;
        virtual void Launch() = 0;
        virtual void Quit() = 0;
        virtual void Propose(const LogValue &log_value)= 0;
        virtual node_id_t GetNodeId() = 0;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
