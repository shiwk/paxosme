//
// Created by k s on 2022/3/27.
//

#include "network.hpp"
#include "communicator.hpp"

void paxosme::Network::Init(paxosme::NetworkConfig network_config) {
    //todo I: too much
    communicator_ = new Communicator(network_config.peers);
}

paxosme::PaxCommunicator *paxosme::Network::GetCommunicator() {
    return communicator_;
}
