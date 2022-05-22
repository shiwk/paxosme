//
// Created by k s on 2022/3/27.
//

#include "network_impl.hpp"

namespace paxosme{

    // todo I: another way for network inteface
    Communicator* NetworkImpl::GetCommunicator() {
        return reinterpret_cast<Communicator *>(communicator);
    }


    NetworkImpl::~NetworkImpl() {};

    void NetworkImpl::Join(paxosme::Node *node) {
//        Network::Join(node);
    }

    void NetworkImpl::Quit(paxosme::Node *node) {
//        Network::Quit(node);
    }

    void NetworkImpl::Init() {

    }
}


