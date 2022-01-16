//
// Created by k s on 2021/9/5.
//
#include <learner_pax.hpp>

namespace paxosme {

    PaxLearner::PaxLearner(const PaxConfig *config, const PaxCommunicator *communicator, const Storage *storage,
                           const Schedule *schedule)
            : PaxPlayer(config, communicator, storage, schedule), learner_state_(config) {}

    int PaxLearner::SHALLILEARN_DELAY = SHALLILEARN_TIMEOUT_CONST;

    void PaxLearner::Init(const PaxController *controller) {
        PaxPlayer::InitController(controller);
        learner_send_loop_ = std::async(std::launch::async, &PaxLearner::SendingLoop, this);
        Publish(EventType::kShallILearnTimeout, [this] { ShallILearn(); }, SHALLILEARN_DELAY);
        // todo I: need publish this kShallILearnTimeout event in other case
    }

//    bool PaxLearner::Learned() {
//        return ;
//    }

    bool PaxLearner::HandleSenderPublish(const PaxMessage &pax_message) {
        LearnFromSelf(pax_message);
        return true;
    }

    void PaxLearner::HandleOthersPublish(const PaxMessage &pax_message) {
        LearnFromOthers(pax_message);
    }
}