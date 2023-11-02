//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_SERVER_HPP
#define PAXOSME_NETWORK_SERVER_HPP

#include "common.hpp"
#include "network.hpp"

namespace paxosme {

    class NetworkServer {
    public:
        static NetworkServer* New();
        virtual void Start(const Peer &, Network::MsgCallback) = 0;
        virtual void Shutdown() = 0;
        virtual ~ NetworkServer () = default;;
        virtual bool Running() = 0;

    protected:
        Network::MsgCallback msgCallback_;
    };
}
#endif //PAXOSME_NETWORK_SERVER_HPP
