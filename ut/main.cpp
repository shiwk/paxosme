//
// Created by k s on 2022/5/22.
//

#include "network.hpp"

int main(){
    auto network = paxosme::Network::New();
    auto nodeIdList =  new paxosme::NodeIdList;
    network->Start(nodeIdList, {"127.0.0.1", 8001});
}