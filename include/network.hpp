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

    class Comm{};
    template<class M>
    class Communicator : public Comm{
    public:
        int Send(node_id_t node_id, const M &message) ;
        int Broadcast(const M &message);
    };

    class Network {
    public:

        static Network *New();

        static void Delete(Network *);

//        Network(PeerList *);
        virtual ~Network() = default;

        void Init();

        void Join(paxosme::Node *);

        void Quit(paxosme::Node *);
    };
}

#endif
//PAXOSME_GRPC_NETWORK_HPP
