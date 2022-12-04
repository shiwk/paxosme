//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_IMPL_HPP
#define PAXOSME_NETWORK_IMPL_HPP

#include "common.hpp"
#include "network.hpp"
#include "controller.hpp"

namespace paxosme {
    using MsgCallback = std::function<void(PaxMessage)>;
//    typedef   void MsgCallback(PaxMessage);

    class ServerInterface {
    public:
        virtual void Start(const Endpoint &, MsgCallback) = 0;

    protected:
        MsgCallback msgCallback_;
    };

    class NetworkImpl : public Network {
        friend class Network;

    private:
        PeerList peers_;
        std::unique_ptr<ServerInterface> server_;
    protected:
        PaxController *paxController_;
    };
}
#endif //PAXOSME_NETWORK_IMPL_HPP
