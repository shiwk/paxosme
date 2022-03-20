//
// Created by k s on 2021/9/5.
//
#include <learner_pax.hpp>

namespace paxosme {

    PaxLearner::PaxLearner(const PaxConfig *config, const PaxCommunicator *communicator, const Storage *storage,
                           const Schedule *schedule)
            : PaxPlayer(config, communicator, storage, schedule), learner_state_(config) {
        job_status_ = kStale;
        highest_known_instance_id_ = INSTANCE_DUMMY;
        receiver_ = NODE_DUMMY;
        begin_instance_id_ = INSTANCE_DUMMY;
        ack_send_ = INSTANCE_DUMMY;

        is_sending_ = false;
        is_learning_ = false;
    }

    int PaxLearner::shall_Learn_delay_ = SHALL_LEARN_TIMEOUT_CONST;

    void PaxLearner::Init(const PaxController *controller) {
        PaxPlayer::InitController(controller);
        learner_send_loop_ = std::async(std::launch::async, &PaxLearner::SendingLoop, this);
        Publish(EventType::kShallILearnTO, [this] { ShallLearn(); }, shall_Learn_delay_);
    }

    bool PaxLearner::HandleSenderPublish(const PaxMessage &pax_message) {
        LearnFromSelf(pax_message);
        return true;
    }

    void PaxLearner::HandleOthersPublish(const PaxMessage &pax_message) {
        LearnFromOthers(pax_message);
    }

    bool PaxLearner::Learned() const {
        return learner_state_.Learned();
    }

    void PaxLearner::HandleMessage(const PaxMessage &message) {
        switch (message.GetMessageType()) {
            case kLEARNER_SENDER_PUBLISH_CHOSEN_VALUE: {
                if (!HandleSenderPublish(message)) {
                    // acceptor failed
                    break;
                }

                if (!HandleSenderPublish(message)) {
                    // learner failed
                    break;
                }
            }
            case kLEARNER_BROADCAST_CHOSEN :
                HandleOthersPublish(message);
                break;
            case kLEARNER_SHALL_I_LEARN:
                HandleShallILearn(message);
                break;
            case kLEARNER_CONFIRM_LEARN:
                HandleConfirmLearn(message);
                break;
            case kLEARNER_SEND_VALUE:
            case kLEARNER_VALUE_SYNC:
                HandleOthersPublish(message);
                break;
            case kLEARNER_VALUE_ACK:
                HandleValueAck(message);
                break;
            case kLEARNER_TELL_INSTANCE_ID:
                HandleTellNewInstanceId(message);
                break;
            default:
                break;
        }
    }
}