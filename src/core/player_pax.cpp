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

    bool PaxPlayer::IsAccepted(const instance_id_t instance_id) {
        return controller_->IsAccepted(instance_id);
    }

    void PaxPlayer::SendMessage(const PaxMessage &pax_message, node_id_t node_id) {
        communicate_->Send(node_id, pax_message);
    }

    void PaxPlayer::BroadCastMessage(const PaxMessage &message) {
        communicate_->Broadcast(message);
    }

    void PaxPlayer::ProcessChosenValue(const PaxMessage &message) {
        controller_->HandleMessage(message);
    }

    void PaxPlayer::WriteState(const PaxosState &paxos_state) {
        storage_->Write(paxos_state.instance_id(), paxos_state);
    }

    PaxosState PaxPlayer::ReadState(instance_id_t instance_id) {
        PaxosState paxos_state;
        storage_->Read(instance_id, paxos_state);
        return paxos_state;
    }

    void PaxPlayer::Publish(EventType event_type, const event_callback &callback, int delayInMilli) {
        const std::chrono::duration<int> delay(delayInMilli);
        event_time t = std::chrono::system_clock::now();
        t.operator+=(delay);
        schedule_->add(callback, t, event_type);
    }

    ProposalTriplet PaxPlayer::GetAcceptedProposal() {
        return controller_->GetAcceptedProposal();
    }

    const LogValue &PaxPlayer::GetAcceptedValue() {
        return controller_->GetAcceptedValue();
    }
}




