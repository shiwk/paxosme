//
// Created by shiwk on 2020/7/18.
//

#include <player_pax.hpp>
#include <controller.hpp>

namespace paxosme {
    instance_id_t PaxPlayer::GetInstanceId() const {
        return controller_->GetInstanceId();
    }

    node_id_t PaxPlayer::GetNodeId() const {
        return node_id_;
    }

    void PaxPlayer::ProcessAcceptedMessage(PaxMessage *pax_message) {
        controller_->Pick(pax_message);
    }

    bool PaxPlayer::IsAccepted(const instance_id_t instance_id) {
        return controller_->IsAccepted(instance_id);
    }

    void PaxPlayer::Persist(const PaxMessage &pax_message) {
        storage_->Write(pax_message.GetInstanceId(), pax_message);
    }

    void PaxPlayer::SendMessage(const PaxMessage &pax_message, node_id_t node_id) {
        communicate_->Send(node_id, pax_message);
    }

    void PaxPlayer::BroadCastMessage(const PaxMessage &message) {
        communicate_->Broadcast(message);
    }

    void PaxPlayer::ProcessChosenValue(const PaxMessage &message) {
        controller_->HandleReceivedMessage(message);
    }
}




