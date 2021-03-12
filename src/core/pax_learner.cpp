//
// Created by shiwk on 2020/7/18.
//

#include <pax_learner.hpp>

const paxosme::LogValue &paxosme::PaxLearner::GetLearnedValue() {
    return learner_state_->GetLearnedValue();
}

void paxosme::PaxLearner::HandleNewValueReceived(const paxosme::PaxMessage &pax_message) {
    instance_id_t instance_id = pax_message.GetInstanceId();
    if (instance_id != GetInstanceId())
        return;
    learner_state_->LearnNew(pax_message.GetLogValue(), pax_message.GetInstanceId(), pax_message.GetProposalId(),
                             pax_message.GetProposer());

    // persist needed since learned from other learners
    Persist(pax_message);
}

void paxosme::PaxLearner::HandleNewValueProposed(const paxosme::PaxMessage &pax_message) {
    instance_id_t instance_id = pax_message.GetInstanceId();
    if (instance_id != GetInstanceId())
        return;

    if (!IsAccepted(instance_id))
        return; // reject if not ever accepted

    // persist not needed since acceptor already persisted
    learner_state_->LearnNew(pax_message.GetLogValue(), instance_id, pax_message.GetProposer(),
                             pax_message.GetProposer());
}

void paxosme::PaxLearner::RequestLearn() {
    node_id_t node_id = GetNodeId();
    node_id_t following_node_id = GetFollowingNodeId();
    instance_id_t instance_id = GetInstanceId();
    PaxMessage new_value_request(node_id, MessageType::LearnerNewRequest);
    new_value_request.SetFollowingNodeId(following_node_id);
    new_value_request.SetInstanceId(instance_id);
    SendMessage(new_value_request, following_node_id);
}

void paxosme::PaxLearner::HandleRequestLearn(const paxosme::NewValueRequest &new_value_request) {
    SetPossibleHigherInstanceId(new_value_request.GetInstanceId());

    if (new_value_request.GetFollowingNodeId() == GetNodeId()) {
        // todo : my follower
    }

    // todo: be careful race condition for learner state
    if (new_value_request.GetInstanceId() + 1 != GetInstanceId()) {
        // instance not matched
        // tell my instance id
        TellInstanceId(GetInstanceId(), new_value_request.GetNodeId());
        return;
    }

    ReplyLearning(learner_state_->GetLearned(), new_value_request.GetNodeId());
}

void paxosme::PaxLearner::SetPossibleHigherInstanceId(const instance_id_t &instance_id) {
    if (instance_id <= highest_known_instance_id_)
        return;
    highest_known_instance_id_ = instance_id;
}

void paxosme::PaxLearner::HandleTellNewInstanceId(const instance_id_t instance_id, const node_id_t node_id) {
    SetPossibleHigherInstanceId(instance_id);
}

void paxosme::PaxLearner::ReplyLearning(const paxosme::PaxMessage &pax_message, node_id_t node_id) {
    SendMessage(pax_message, node_id);
}

void paxosme::PaxLearner::HandleReplyLearning(const paxosme::PaxMessage &pax_message) {
    if (pax_message.GetInstanceId() != GetInstanceId()) {
        // it is not learnabel if higher than local instance_id
        // it is not needed if lower than local instance_id
        return;
    }

    // learn new value
    learner_state_->LearnNew(pax_message.GetLogValue(), pax_message.GetInstanceId(), pax_message.GetProposalId(),
                             pax_message.GetProposer());
    // todo: whether needs Learning ack
}

void paxosme::PaxLearner::TellInstanceId(const instance_id_t instance_id, const node_id_t node_id) {
    SendInstanceId(instance_id, node_id);
    // todo: whether needs confirm value learned
}

void paxosme::PaxLearner::TellLearnedToFollowers(const PaxMessage &message) {
    SendFollowers(message, MessageType::Choosen);
}

void paxosme::PaxLearner::LearnBySelf(const paxosme::PaxMessage &pax_message) {
    if (pax_message.GetInstanceId() != GetInstanceId())
        return; // instance id not matched
    if (pax_message.GetProposer() != GetNodeId())
        return; // proposer not matched

    learner_state_->LearnNew(pax_message.GetLogValue(), pax_message.GetInstanceId(), pax_message.GetProposalId(),
                             pax_message.GetProposer());
    TellLearnedToFollowers(pax_message); // tell followers
}

bool paxosme::PaxLearner::AnymoreToLearn() {
    return GetInstanceId() + 1 < highest_known_instance_id_;
}


