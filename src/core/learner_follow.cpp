//
// Created by shiwk on 2020/7/18.
//

#include <learner_pax.hpp>

namespace paxosme {
    const LogValue &PaxLearner::GetLearnedValue() {
        return learner_state_.GetLearnedValue();
    }

    void PaxLearner::ShallLearn() {
        // re-launch message
        Publish(EventType::kShallILearnTO, [this] { ShallLearn(); }, shall_Learn_delay_);

        node_id_t node_id = GetNodeId();
        instance_id_t instance_id = GetInstanceId();

        PaxMessage message(node_id, MessageType::kShallILearn);
        message.SetInstanceId(instance_id);
        BroadCastMessage(message);
    }


    void PaxLearner::SetPossibleHigherInstanceId(const instance_id_t &instance_id) {
        if (instance_id <= highest_known_instance_id_)
            return;
        highest_known_instance_id_ = instance_id;
    }

    void PaxLearner::HandleTellNewInstanceId(const PaxMessage &pax_message) {
        SetPossibleHigherInstanceId(pax_message.GetLeaderInstanceId());

        if (pax_message.GetInstanceId() != GetInstanceId()) {
            // ignore message because out of date
            return;
        }

        if (pax_message.GetLeaderInstanceId() < GetInstanceId()) {
            // ignore because it does not lead on me
            return;
        }
        // todo II : compare checkpoint and sync checkpoint if needed

        std::unique_lock<std::mutex> lck(mutex_follow_);
        if (!is_learning_)
            ConfirmLearn(pax_message.GetSender());
    }

    void PaxLearner::ConfirmLearn(node_id_t node_id) {
        std::unique_lock<std::mutex> lck(mutex_follow_);
        is_learning_ = true;
        PaxMessage pax_message(node_id, MessageType::kConfirmLearn);
        pax_message.SetInstanceId(GetInstanceId());
    }

    bool PaxLearner::AnymoreToLearn() {
        return GetInstanceId() + 1 < highest_known_instance_id_;
    }

    void PaxLearner::LearnFromOthers(const PaxMessage &pax_message) {
        // re-launch message
        Publish(EventType::kShallILearnTO, [this] { ShallLearn(); }, shall_Learn_delay_);

        if (pax_message.GetInstanceId() != GetInstanceId())
            return; // instance id not matched

        LearnNew(pax_message.GetChosenValue(), pax_message.GetInstanceId(), pax_message.GetProposalId(),
                 pax_message.GetProposer(), true);

        if (pax_message.GetMessageType() == MessageType::kValue_SYNC)
            Ack(pax_message.GetSender());
    }

    void PaxLearner::Ack(node_id_t node_id) {
        PaxMessage pax_message(GetNodeId(), MessageType::kValue_ACK);
        pax_message.SetInstanceId(GetInstanceId());
        SendMessage(pax_message, node_id);
    }

    void PaxLearner::NewInstance() {
        learner_state_.Reset();
    }
}