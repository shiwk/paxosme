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
    struct NodeOptions {
        node_id_vector nodeIdList;
        node_id_t nodeId;
        std::shared_ptr<LogStorage> storage;
        std::shared_ptr<Statemachine> sm;
        std::shared_ptr<Network> network;
    };

    class Node {
    public:

        static void Run(NodeOptions &, PaxosOptions &, const std::shared_ptr<Node> &);

        void Init(node_id_t, PaxosOptions &);

    private:
        void InitNetwork(Network::NetworkOptions);

        void InitSM(Statemachine::SmOptions);

        void InitStorage(LogStorage::LogStorageOptions);

        node_id_t nodeId_;
        std::shared_ptr<Network> network_;
        std::shared_ptr<LogStorage> storage_;
        std::shared_ptr<Statemachine> sm_;
        // std::shared_ptr<PaxController> paxController_;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
