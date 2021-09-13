//
// Created by k s on 2021/9/5.
//
#include <learner_pax.hpp>

namespace paxosme {
    int PaxLearner::SHALLILEARN_DELAY = SHALLILEARN_TIMEOUT_CONST;

    void PaxLearner::Init() {
        learner_send_loop_ = std::async(std::launch::async, &PaxLearner::SendingLoop, this);
        auto callback= [this]{ShallILearn();};
        Publish(EventType::kShallILearnTimeout, callback, SHALLILEARN_DELAY);
        // todo I: need publish this kShallILearnTimeout event in other case
    }

//    bool PaxLearner::Learned() {
//        return ;
//    }

    bool PaxLearner::HandleSenderPublish(const PaxMessage &pax_message) {
        LearnFromSelf(pax_message);
        return true;
    }

    void PaxLearner::HandleOthersPublish(const PaxMessage& pax_message) {
        LearnFromOthers(pax_message);
    }
}