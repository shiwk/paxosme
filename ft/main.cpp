//
// Created by k s on 2022/5/22.
//

#include "network.hpp"
#include "node.hpp"
#include <segment_store.hpp>
#include "sm.hpp"

int main(){
    paxosme::NodeOptions nodeOptions;
    nodeOptions.network = std::unique_ptr<paxosme::Network> (paxosme::Network::New());
    nodeOptions.storage = std::shared_ptr<paxosme::LogStorage>();

    Statemachine::SmOptions smOptions;
    auto sm = Statemachine::New();
    sm->Init(smOptions);

    nodeOptions.sm = std::shared_ptr<Statemachine>(sm);
//    leveldb::DB * levelDB = nullptr;
//    leveldb::Options oOptions;
//    oOptions.create_if_missing = true;
//    auto status =  leveldb::DB::Open(oOptions, "sDBPath", &levelDB);

    auto logSegmentStore = LogSegmentStore::New();
    
    paxosme::PaxosOptions paxos_options;
    std::shared_ptr<paxosme::Node> node = std::make_shared<paxosme::Node>();
    paxosme::Node::Run(nodeOptions, paxos_options, node);
}