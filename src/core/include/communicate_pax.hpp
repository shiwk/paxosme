//
// Created by shiwk on 2021/3/6.
//

#ifndef PAXOSME_COMMUNICATE__PAXHPP
#define PAXOSME_COMMUNICATE__PAXHPP

#include "messages_pax.hpp"

namespace paxosme {
    class PaxCommunicate {
        virtual int Send(node_id_t node_id, const PaxMessage &pax_message) = 0;
        virtual int Receive(const PaxMessage &pax_message) = 0;
    };
}
#endif //PAXOSME_COMMUNICATE__PAXHPP
