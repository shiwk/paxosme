//
// Created by shiwk on 2020/7/18.
//

#include <player_pax.hpp>
#include <controller.hpp>

namespace paxosme {
    PaxPlayer::PaxPlayer(const ConfigInfo *config, const PaxCommunicator *communicator,
                         const PaxStorage *storage)
            : config_(const_cast<ConfigInfo *>(config)),
              communicator_(const_cast<PaxCommunicator *>(communicator)),
              storage_(const_cast<PaxStorage *>(storage)),
              schedule_(Schedule::OneInstance()) {
    }

    instance_id_t PaxPlayer::GetInstanceId() const {
        return controller_->GetInstanceId();
    }

    node_id_t PaxPlayer::GetNodeId() const {
        return config_->GetNodeId();
    }

    bool PaxPlayer::IsAccepted(const instance_id_t instance_id) {
        return controller_->IsAccepted(instance_id);
    }

    void PaxPlayer::SendMessage(const PaxMessage &pax_message, node_id_t node_id) {
        communicator_->Send(node_id, pax_message);
    }

    void PaxPlayer::BroadCastMessage(const PaxMessage &message) {
        communicator_->Broadcast(message);
    }

    void PaxPlayer::ProcessChosenValue(const PaxMessage &message) {
        controller_->HandleMessage(message);
    }

    void PaxPlayer::WriteState(const PaxosStorageState &paxos_state) {
        storage_->Write(paxos_state.instanceId, paxos_state);
    }

    PaxosStorageState PaxPlayer::ReadState(instance_id_t instance_id) {
        PaxosStorageState paxos_state;
        storage_->Read(instance_id, paxos_state);
        return paxos_state;
    }

    void PaxPlayer::Publish(EventType event_type, const EventHandler &callback, millisec delayInMilli) {
        const std::chrono::duration<int> delay(delayInMilli);
        EventTimeStamp t = STEADY_TIME_NOW;
        t.operator+=(delay);
        schedule_->AddEvent(callback, t, event_type);
    }

    ProposalTriplet PaxPlayer::GetAcceptedProposal() {
        return controller_->GetAcceptedProposal();
    }

    const LogValue &PaxPlayer::GetAccepted() {
        return controller_->GetAcceptedValue();
    }

    void PaxPlayer::InitController(const PaxController *controller) {
        controller_ = const_cast<PaxController *> (controller);
    }

    void PaxPlayer::Withdraw(EventType event_type) {
        schedule_->Remove(event_type);
    }
}




