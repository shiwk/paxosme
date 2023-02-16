//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_SERVER_HPP
#define PAXOSME_NETWORK_SERVER_HPP

#include "common.hpp"
#include "network.hpp"
#include "messages_pax.hpp"

namespace paxosme {
    using MsgCallback = std::function<void(PaxMessage)>;
//    typedef   void MsgCallback(PaxMessage);

    class NetworkServer {
    public:
        static NetworkServer *New();
        virtual void Start(const Endpoint &, MsgCallback) = 0;

    protected:
        MsgCallback msgCallback_;
    };
}
#endif //PAXOSME_NETWORK_SERVER_HPP
