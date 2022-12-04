//
// Created by kk on 2022/12/4.
//

#include "node.hpp"

namespace paxosme{
    void Node::Run(NodeOptions &options, PaxosOptions &paxos_options, const std::shared_ptr<Node> &node) {
        // network&storage&sm init
        options.network->Start(options.nodeIdList, paxos_options.GetNodeId());
        options.sm->Init();
        options.storage->Init();
        node->Init(paxos_options.GetNodeId());
    }

    void Node::Init(node_id_t node_id) {
        nodeId_ = node_id;
    }
}