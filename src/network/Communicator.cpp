//
// Created by kk on 2023/2/25.
//

#include "network.hpp"
#include "messages.hpp"

namespace paxosme {
    template<>
    int Communicator<PaxMessage>::SendMessage(node_id_t node_id, const PaxMessage &message) {
        network_->Send(Network::NodeIdToPeer(node_id), message);
        return 0;
    }

    template<>
    int Communicator<PaxMessage>::BroadcastMessage(const paxosme::PaxMessage &message) {
        network_->Broadcast(message);
        return 0;
    }
}