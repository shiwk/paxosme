//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_NODE_HPP
#define PAXOSME_NODE_HPP

#include <string>
#include <vector>
#include "config.hpp"
#include "logstorage.hpp"
#include "sm.hpp"
#include "network.hpp"

namespace paxosme {
    struct NodeOptions{
        std::shared_ptr<LogStorage> storage;
        std::shared_ptr<StateMachine> sm;
        node_id_vector nodeIdList;
        std::unique_ptr<Network> network;

    };

    class Node {
    public:
        Node()=delete;
        static void Run(NodeOptions &, PaxosOptions &, const std::shared_ptr<Node> &);
        void Init(node_id_t);
        void Quit();
    private:
        node_id_t nodeId_;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
