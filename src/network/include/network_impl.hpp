//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_IMPL_HPP
#define PAXOSME_NETWORK_IMPL_HPP

#include "common.hpp"
#include "network.hpp"


namespace paxosme {
    class NetworkImpl : public Network {
    public:

        Communicator *GetCommunicator() override;
        void Init() override;
        void Join(paxosme::Node *node) override;

        void Quit(paxosme::Node *node) override;

        ~NetworkImpl() override;

    private:
        Communicator *communicator;
    };
}
#endif //PAXOSME_NETWORK_IMPL_HPP
