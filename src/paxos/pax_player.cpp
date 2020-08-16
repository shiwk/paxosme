//
// Created by shiwk on 2020/7/18.
//

#include <pax_player.hpp>
#include <controller.hpp>

paxosme::PaxMessage paxosme::PaxPlayer::GeneratePreMessage() const {
    return {pax_state_->GetInstanceId(), pax_state_->GetApplicableProposalId(), node_id_};
}

instance_id_t paxosme::PaxPlayer::GetInstanceId() {
    return pax_state_->GetInstanceId();
}

void paxosme::PaxPlayer::ProcessMessageAcceptedByMajority(paxosme::PaxMessage &pax_message) {
    controller_->AddMessage(pax_message);
}

void paxosme::PaxPlayer::BroadCastToAcceptors(const PaxRequest &pax_request) {
    communicator_->BroadCast(pax_request);
}


