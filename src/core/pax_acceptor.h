//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_ACCEPTOR_H
#define PAXOSME_PAX_ACCEPTOR_H

#include "player.h"

namespace paxosme {
    class PaxAcceptor : public PaxPlayer{
        void AcceptPrePropose(PaxMessage message);

        void AcceptPropose(PaxMessage message);
    private:

    };
}

#endif //PAXOSME_PAX_ACCEPTOR_H
