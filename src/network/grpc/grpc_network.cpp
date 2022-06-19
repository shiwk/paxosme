//
// Created by k s on 2022/3/27.
//

#include "network_impl.hpp"

namespace paxosme {

    // todo I: another way for network inteface

    void Network::Join(paxosme::Node *node) {
        auto *impl = (NetworkImpl *) this;
        // todo I: impl
    }

    void Network::Quit(paxosme::Node *node) {
        auto *impl = (NetworkImpl *) this;
        // todo I: impl
    }

    void Network::Init() {
        auto *impl = (NetworkImpl *) this;
        // todo I: impl
    }

    Network *Network::New() {
        return new NetworkImpl;
    }

    void Network::Delete(Network *network) {
        delete network;
    }
}


