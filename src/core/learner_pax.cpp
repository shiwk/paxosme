//
// Created by k s on 2021/9/5.
//
#include <learner_pax.hpp>

namespace paxosme {
    void PaxLearner::Init() {
        learner_send_loop_ = std::async(std::launch::async, &PaxLearner::SendingLoop, this);
        // todo I : add event schedule for AskForLearn
    }

    bool PaxLearner::Learned() {
        return false;
    }

    bool PaxLearner::HandleSenderPublish(const PaxMessage &pax_message) {
        LearnFromSelf(pax_message);
        return true;
    }

    void PaxLearner::HandleOthersPublish(const PaxMessage& pax_message) {
        LearnFromOthers(pax_message);
    }
}