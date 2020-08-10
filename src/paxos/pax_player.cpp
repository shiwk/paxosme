//
// Created by shiwk on 2020/7/18.
//

#include <pax_player.hpp>
#include <controller.hpp>

void paxosme::PaxPlayer::BroadCast(const PaxMessage &message) {
    PaxRequest pax_request = message;
    communicator_->BroadCast(pax_request);
}

paxosme::PaxMessage paxosme::PaxPlayer::GenerateMessage(const paxosme::LogValue &log_value) const {
    PaxMessage pax_message = PaxMessage(pax_state_->GetInstanceId(), pax_state_->GetApplicableProposalId(),
                                        node_id_, log_value);
    return pax_message;
}

instance_id_t paxosme::PaxPlayer::GetInstanceId() {
    return pax_state_->GetInstanceId();
}

void paxosme::PaxPlayer::ProcessMessageAcceptedByMajority(paxosme::PaxMessage &pax_message) {
    controller_->AddMessage(pax_message);
}

paxosme::PaxRequest::PaxRequest(const paxosme::PaxMessage &message) : PaxMessage(message.GetProposalId()){

}
