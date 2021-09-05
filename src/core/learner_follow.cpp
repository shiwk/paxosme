//
// Created by shiwk on 2020/7/18.
//

#include <learner_pax.hpp>

namespace paxosme {
    const LogValue &PaxLearner::GetLearnedValue() {
        return learner_state_->GetLearnedValue();
    }

    void PaxLearner::ShallILearn() {
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
            ConfirmLearn(pax_message.GetSenderId());
    }

    void PaxLearner::ConfirmLearn(node_id_t node_id) {
        std::unique_lock<std::mutex> lck(mutex_follow_);
        is_learning_ = true;
        PaxMessage pax_message(node_id, MessageType::kSendValue);
        pax_message.SetInstanceId(GetInstanceId());
    }

    bool PaxLearner::AnymoreToLearn() {
        return GetInstanceId() + 1 < highest_known_instance_id_;
    }

    void PaxLearner::Init() {
        learner_send_loop_ = std::async(std::launch::async, &PaxLearner::SendingLoop, this);
    }

    void PaxLearner::LearnFromOthers(const PaxMessage &pax_message) {
        if (pax_message.GetInstanceId() != GetInstanceId())
            return; // instance id not matched

        LearnNew(pax_message.GetChosenValue(), pax_message.GetInstanceId(), pax_message.GetProposalId(),
                 pax_message.GetProposer(), true);
    }
}