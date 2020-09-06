//
// Created by shiwk on 2020/7/18.
//

#include <pax_player.hpp>
#include <controller.hpp>


instance_id_t paxosme::PaxPlayer::GetInstanceId() const {
    return controller_->GetInstanceId();
}

node_id_t paxosme::PaxPlayer::GetNodeId() const {
    return node_id_;
}

void paxosme::PaxPlayer::ProcessMessageAcceptedByMajority(paxosme::PaxMessage &pax_message) {
    controller_->AddMessage(pax_message);
}



