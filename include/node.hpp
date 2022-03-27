//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_NODE_HPP
#define PAXOSME_NODE_HPP

#include <string>
#include <vector>
#include "config.hpp"

namespace paxosme {
    class Node {
    public:
        virtual ~Node() {}

        void Start();

        void Propose(const LogValue &log_value, instance_id_t &instance_id);

        node_id_t GetNodeId();

    private:
        Peer peer_;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
