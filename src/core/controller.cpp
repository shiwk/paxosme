//
// Created by shiwk on 2020/8/1.
//

#include <controller.hpp>

#define HARDCODE_MSG_COUNT_LIMIT 1000

namespace paxosme {
    PaxController::PaxController(const PaxConfig *config, const PaxCommunicator *communicator, const Storage *storage,
                                 const Schedule *schedule)
            : pax_config_(const_cast<PaxConfig *>(config)), msgProv_(HARDCODE_MSG_COUNT_LIMIT) {
        proposer_ = new PaxProposer{config, communicator, storage, schedule};
        learner_ = new PaxLearner{config, communicator, storage, schedule};
        acceptor_ = new PaxAcceptor{config, communicator, storage, schedule};
    }

    instance_id_t PaxController::GetInstanceId() const {
        return instance_id_;
    }

    bool PaxController::IsAccepted(const instance_id_t i) {
        return acceptor_->IsAccepted();
    }

    const LogValue &PaxController::GetAcceptedValue() {
        return acceptor_->GetAcceptedValue();
    }

    node_id_t PaxController::GetAcceptedNodeId() {
        return acceptor_->GetAcceptedNodeId();
    }

    ProposalTriplet PaxController::GetAcceptedProposal() {
        proposal_id_t proposalId = acceptor_->GetAcceptedProposalId();
        node_id_t nodeId = acceptor_->GetAcceptedNodeId();
        return {GetInstanceId(), proposalId, nodeId};
    }

    void PaxController::Init() {
        instance_id_t instanceInState = acceptor_->Init(this);
        instance_id_t instanceInSM = state_machine_->GetInstanceId();

        if (instanceInState > instanceInSM + 1) {
            // if instance_id > instance id in checkpoint, it means needs to execute some log_values
            PushSMByState(instanceInState);
            instance_id_ = instanceInState;
        }

        if (instanceInState < instanceInSM) {
            // todo III : if instance_id < instance id in checkpoint, it means some special cases (like restart/crash, even worse) happened
            instance_id_ = instanceInSM + 1;
        }

        prov_loop_ = std::async(std::launch::async, [this] { return MainLoop(); });
        proposal_id_t proposalId = acceptor_->GetAcceptedProposalId();

        proposer_->Init(proposalId + 1, this);
        learner_->Init(this);
    }

    void PaxController::TryNewValue() {
        if (learner_->AnymoreToLearn()){
            // not catching up
            return;
        }

        if (!pax_config_->ProposingAuthority()) {
            // not authorized
            return;
        }

        proposer_->NewValue();
    }

    void PaxController::PushSMByState(instance_id_t target_instance_id) {
        instance_id_t instanceInSM = state_machine_->GetInstanceId();
        for (int i = instanceInSM; i < target_instance_id; ++i) {
            PaxosState paxos_state = acceptor_->ReadState(i);
            state_machine_->Execute(i, paxos_state.accepted_value());
        }
    }

    void PaxController::HandleMessage(const PaxMessage &message) {
        switch (message.GetMessageType()) {
            // for proposer
            case kPrepareReply: {
                proposer_->HandlePrepareReply(message);
                break;
            }
            case kProposeReply:
                proposer_->HandleProposeReply(message);
                break;

                // for acceptor
            case kPrepareBroadCast:
                acceptor_->HandlePrepareRequest(message);
                break;

            case kProposeBroadCast:
                acceptor_->HandleProposeRequest(message);
                break;

                //for learner
            case kSenderPublishChosenValue: {
                if (!acceptor_->HandleSenderPublish(message)) {
                    // acceptor failed
                    break;
                }

                if (!learner_->HandleSenderPublish(message)) {
                    // learner failed
                    break;
                }
            }
            case kBroadCastChosen :
                learner_->HandleOthersPublish(message);
                break;
            case kShallILearn:
                learner_->HandleShallILearn(message);
                break;
            case kConfirmLearn:
                learner_->HandleConfirmLearn(message);
                break;
            case kSendValue:
            case kValue_SYNC:
                learner_->HandleOthersPublish(message);
                break;
            case kValue_ACK:
                learner_->HandleValueAck(message);
                break;
            case kTellInstanceId:
                learner_->HandleTellNewInstanceId(message);
                break;
            default:
                break;
        }


        if (learner_->Learned()) {
            // todo II: state machine execute
            InstanceDone();
            NewInstance();
        }
    }

    [[noreturn]] void *PaxController::MainLoop() {
        while (true) {

            Event eventToHandle;
            while (schedule_.Dispatch(eventToHandle))
                eventToHandle();

            EventTimeStamp nextEventTime;
            PaxMessage *paxMessage = nullptr;
            if (schedule_.NextEventTime(nextEventTime)) {
                if (msgProv_.Take(paxMessage, Time::DurationMS(nextEventTime, STEADY_TIME_NOW))) {
                    HandleMessage(*paxMessage);
                    delete paxMessage;
                }
            } else if (msgProv_.Take(paxMessage)) {
                HandleMessage(*paxMessage);
                delete paxMessage;
            }

            TryNewValue();
        }
    }

    PaxController::~PaxController() {
        delete learner_;
        learner_ = nullptr;

        delete proposer_;
        proposer_ = nullptr;

        delete acceptor_;
        acceptor_ = nullptr;
    }

    void PaxController::InstanceDone() {
        proposer_->InstanceDone(instance_id_, learner_->GetLearnedValue());
    }

    void PaxController::NewInstance() {
        instance_id_++;
        acceptor_->NewInstance();
        proposer_->NewInstance();
        learner_->NewInstance();
    }

    void PaxController::AddMessage(PaxMessage &pax_message) {
        PaxMessage* message = &pax_message;
        msgProv_.TryAdd(message);
    }

}