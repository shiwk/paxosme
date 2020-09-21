//
// Created by shiwk on 2020/7/18.
//

#include <pax_learner.hpp>

const paxosme::LogValue &paxosme::PaxLearner::GetLearnedValue() {
    return pax_learner_state_->GetLearnedValue();
}

void paxosme::PaxLearner::HandleNewValueRequestReply(const paxosme::PaxMessage &pax_message) {
    instance_id_t instance_id = pax_message.GetInstanceId();
    if (instance_id != GetInstanceId())
        return;
    pax_learner_state_->LearnNewValue(pax_message.GetLogValue());

    // persist needed since learned from other learners
    Persist(instance_id, pax_message.GetLogValue());
}

void paxosme::PaxLearner::HandleNewValueProposed(const paxosme::PaxMessage &pax_message) {
    instance_id_t instance_id = pax_message.GetInstanceId();
    if (instance_id != GetInstanceId())
        return;

    if (!IsAccepted(instance_id))
        return; // reject if not ever accepted
    // persist not needed since acceptor already persisted
    pax_learner_state_->LearnNewValue(pax_message.GetLogValue());
}


