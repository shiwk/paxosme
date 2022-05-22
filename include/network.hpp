//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include <string>
#include <vector>
#include "node.hpp"

namespace paxosme {
    struct Peer {
        std::string ip;
        int port;
    };

    struct PeerList {
        std::vector<Peer> peers;
    };

    class Communicator;

    class Network {
    public:

//        Network(PeerList *);
        virtual ~Network() =default;

        virtual void Init() =0;

        virtual void Join(paxosme::Node *) = 0;
        virtual void Quit(paxosme::Node *) =0 ;
        virtual Communicator* GetCommunicator() = 0;
    };
}

#endif
//PAXOSME_GRPC_NETWORK_HPP
