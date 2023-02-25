//
// Created by k s on 2022/5/22.
//

#include "network.hpp"
#include "node.hpp"

int main(){
    paxosme::NodeOptions node_options;
    node_options.network = std::unique_ptr<paxosme::Network> (paxosme::Network::New());
    node_options.storage = std::shared_ptr<paxosme::Storage>();
    node_options.sm = std::shared_ptr<paxosme::StateMachine>();

    paxosme::PaxosOptions paxos_options;
    std::shared_ptr<paxosme::Node> node = std::make_shared<paxosme::Node>();
    paxosme::Node::Run(node_options, paxos_options, node);
}