//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_COMMUNICATOR_HPP
#define PAXOSME_COMMUNICATOR_HPP

#include <common.hpp>
#include "types/pax_messages.hpp"

namespace paxosme {
    class Communicator {
        void BroadCast(PaxMessage pax_message);
        void Send(PaxMessage pax_message, NodeId node_id);
    };
}

#endif //PAXOSME_COMMUNICATOR_HPP
