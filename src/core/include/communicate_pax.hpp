//
// Created by shiwk on 2021/3/6.
//

#ifndef PAXOSME_COMMUNICATE__PAXHPP
#define PAXOSME_COMMUNICATE__PAXHPP

#include "messages_pax.hpp"
#include "network.hpp"

namespace paxosme {
    class PaxCommunicator {
    public:
        PaxCommunicator(Communicator<PaxMessage> *communicator) : communicator_(communicator) {}

        int Send(node_id_t node_id, const PaxMessage &pax_message);

        int Broadcast(const PaxMessage &pax_message);

    private:
        Communicator<PaxMessage> *communicator_;
    };
}
#endif //PAXOSME_COMMUNICATE__PAXHPP
