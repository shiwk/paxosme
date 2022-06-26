//
// Created by k s on 2022/3/27.
//

#include "network_impl.hpp"
#include "server.hpp"
#include <memory>

namespace paxosme {

    // todo I: another way for network inteface

    Network *Network::New() {
        auto nwImpl = new NetworkImpl;
        nwImpl->server_ = std::unique_ptr<NetworkServer>((NetworkServer *) new GrpcServer);

        return nwImpl;
    }
}


