//
// Created by k s on 2022/5/22.
//

#include "network.hpp"
#include "node.hpp"
#include "sm.hpp"

int main(){
    paxosme::NodeOptions node_options;
    node_options.network = std::unique_ptr<paxosme::Network> (paxosme::Network::New());
    node_options.storage = std::shared_ptr<paxosme::LogStorage>();

    Statemachine::SmOptions smOptions;
    auto sm = Statemachine::New();
    sm->Init(smOptions);

    node_options.sm = std::shared_ptr<Statemachine>(sm);
//    leveldb::DB * levelDB = nullptr;
//    leveldb::Options oOptions;
//    oOptions.create_if_missing = true;
//    auto status =  leveldb::DB::Open(oOptions, "sDBPath", &levelDB);

    paxosme::PaxosOptions paxos_options;
    std::shared_ptr<paxosme::Node> node = std::make_shared<paxosme::Node>();
    paxosme::Node::Run(node_options, paxos_options, node);
}