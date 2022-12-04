//
// Created by k s on 2021/9/5.
//
#include <learner_pax.hpp>

namespace paxosme {

    PaxLearner::PaxLearner(const PaxosOptions *config, const PaxCommunicator *communicator, const PaxStorage *storage)
            : PaxPlayer(config, communicator, storage), learner_state_(config) {
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
        Publish(EventType::kEVENT_SHALL_I_LEARN, [this] { ShallLearn(); }, shall_Learn_delay_);
    }

    bool PaxLearner::HandleSenderPublish(const PaxMessage &pax_message) {
        LearnFromSelf(pax_message);
        // tell others
        TellOthers(pax_message.GetProposalId(), GetNodeId(), pax_message.GetLearnedValue());
        return true;
    }

    void PaxLearner::HandleOthersPublish(const PaxMessage &pax_message) {
        if (pax_message.GetInstanceId() != GetInstanceId())
            return; // instance id not matched
        if (pax_message.GetProposingNodeId() != GetNodeId())
            return; // proposer not matched

        auto proposal = GetAcceptedProposal();
        if (proposal.proposal_id == PROPOSAL_DUMMY || pax_message.GetProposalId() != proposal.proposal_id)
            return; // proposalId == PROPOSAL_DUMMY means not accepted yet

        LearnFromOthers(pax_message);
        // todo II: tell followers
    }

    bool PaxLearner::Learned() const {
        return learner_state_.Learned();
    }

    void PaxLearner::HandleMessage(const PaxMessage &message) {
        switch (message.GetMessageType()) {
            case kMSG_VALUE_CHOSEN:
                HandleSenderPublish(message);

            case kMSG_LEARNER_VALUE_SEND:
            case kMSG_VALUE_CHOSEN_BROADCAST :
                HandleOthersPublish(message);
                break;

            case kMSG_SHALL_I_LEARN:
                HandleShallILearn(message);
                break;

            case kMSG_CONFIRM_LEARN:
                HandleConfirmLearn(message);
                break;

            case kMSG_LEARNER_VALUE_SYNC:
                HandleSyncValue(message);
                break;

            case kMSG_SYNC_VALUE_ACK:
                HandleValueAck(message);
                break;

            case kMSG_TELL_INSTANCE_ID:
                HandleTellNewInstanceId(message);
                break;

            default:
                break;
        }
    }
}