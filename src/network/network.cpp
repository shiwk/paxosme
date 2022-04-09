//
// Created by k s on 2022/3/27.
//

#include "network.hpp"

void paxosme::Network::Init() {
    //todo I: too much
}

paxosme::PaxCommunicator *paxosme::Network::GetCommunicator() {
    return communicator_;
}
