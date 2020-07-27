//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_ACCEPTOR_HPP
#define PAXOSME_PAX_ACCEPTOR_HPP

#include "player.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class PaxAcceptor : public PaxPlayer{
        void AcceptPrePropose(PaxMessage message);

        void AcceptPropose(PaxMessage message);
    private:
        PaxConfig pax_config_;
    public:
        PaxAcceptor(const PaxConfig &pax_config);
    };
}

#endif //PAXOSME_PAX_ACCEPTOR_HPP
