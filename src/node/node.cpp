//
// Created by kk on 2022/12/4.
//

#include "node.hpp"


namespace paxosme {
    void Node::Run(NodeOptions &options, PaxosOptions &paxos_options, const std::shared_ptr<Node> &node) {
        // network&storage&sm init

        node->Init(options.nodeId, paxos_options);

        auto networkOption = Network::NetworkOptions{
                options.nodeIdList,
                paxos_options.GetNodeId(),
        };
        node->network_ = options.network;
        node->InitNetwork(networkOption);

        node->storage_ = options.storage;

        node->sm_ = options.sm;
    }

    void Node::Init(node_id_t, PaxosOptions &paxos_options) {
        paxController_ = std::make_shared<PaxController>(PaxController(&paxos_options));
    }

    void Node::InitNetwork(Network::NetworkOptions network_options) {
        network_->Start(network_options);
    }

    void Node::InitSM(StateMachine::SMOptions) {
        // todo II
    }

    void Node::InitStorage(Storage::StorageOptions) {
        // todo II
    }


}