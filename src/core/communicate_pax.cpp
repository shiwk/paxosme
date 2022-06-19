//
// Created by k s on 2022/6/19.
//

#include "communicate_pax.hpp"

namespace paxosme {
    int PaxCommunicator::Send(node_id_t node_id, const PaxMessage &pax_message) {
        communicator_->Send(node_id, pax_message);
    }

    int PaxCommunicator::Broadcast(const PaxMessage &pax_message) {
        communicator_->Broadcast(pax_message);
    }
}