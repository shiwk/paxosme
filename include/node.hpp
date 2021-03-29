//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_NODE_HPP
#define PAXOSME_NODE_HPP

#include <string>
#include <vector>
#include "common.hpp"

namespace paxosme {
    class NodeInfo {
        node_id_t node_id_;
        std::string ip_;
        int port_;
    };

    class NodeList {
        std::vector<NodeInfo *> node_list_;
    };

    class Node {
    public:
        virtual ~Node() {}

        void Start();

        void Propose(const LogValue &log_value, instance_id_t &instance_id);

        node_id_t GetNodeId();

    private:
        NodeInfo node_info_;
        NodeList my_learners_;
        NodeList all_nodes_;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
