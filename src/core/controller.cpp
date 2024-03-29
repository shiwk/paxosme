//
// Created by shiwk on 2020/8/1.
//

#include <controller.hpp>
#include <schedule.hpp>

#define HARDCODE_MSG_COUNT_LIMIT 1000

namespace paxosme
{

    inline bool MessageForProposer(MessageType message_type)
    {
        return message_type >> 4 == 0x1;
    }

    static inline bool MessageForAcceptor(MessageType message_type)
    {
        return message_type >> 3 == 0x1;
    }

    static inline bool MessageForLearner(MessageType message_type)
    {
        return message_type >> 5 == 0x1;
    }

    static inline bool MessagePlaceHolder(MessageType message_type)
    {
        return message_type >> 6 == 0x1;
    }

    PaxController::PaxController(const PaxosOptions *config)
        : pax_config_(const_cast<PaxosOptions *>(config)),
          msgProv_(new SafeQueue<PaxMessage *>(HARDCODE_MSG_COUNT_LIMIT))
    {
    }

    instance_id_t PaxController::GetInstanceId() const
    {
        return instance_id_;
    }

    bool PaxController::IsAccepted(const instance_id_t i)
    {
        return acceptor_->IsAccepted();
    }

    const LogValue &PaxController::GetAcceptedValue()
    {
        return acceptor_->GetAcceptedValue();
    }

    node_id_t PaxController::GetAcceptedNodeId()
    {
        return acceptor_->GetAcceptedNodeId();
    }

    ProposalTriplet PaxController::GetAcceptedProposal()
    {
        proposal_id_t proposalId = acceptor_->GetAcceptedProposalId();
        node_id_t nodeId = acceptor_->GetAcceptedNodeId();
        return {GetInstanceId(), proposalId, nodeId};
    }

    void PaxController::Init(Comm *comm, PaxStore *storage)
    {
        communicator_ = new PaxCommunicator((Communicator<PaxMessage> *)comm);

        proposer_ = new PaxProposer{pax_config_, communicator_, storage};
        learner_ = new PaxLearner{pax_config_, communicator_, storage};
        acceptor_ = new PaxAcceptor{pax_config_, communicator_, storage};

        instance_id_t instanceInState = acceptor_->Init(this);
        instance_id_t instanceInSM = state_machine_->GetSMInstanceId();

        if (instanceInState > instanceInSM + 1)
        {
            // if instance_id > instance id in checkpoint, it means needs to execute some log_values
            PushSMByState(instanceInState);
            instance_id_ = instanceInState;
        }

        if (instanceInState < instanceInSM)
        {
            // todo III : if instance_id < instance id in checkpoint, it means some special cases (like restart/crash, even worse) happened
            instance_id_ = instanceInSM + 1;
        }

        auto mainLoop = [this](void *ptr)
        { return MainLoop(ptr); };
        PaxController::prov_loop = std::async(std::launch::async, mainLoop, nullptr);
        proposal_id_t proposalId = acceptor_->GetAcceptedProposalId();

        proposer_->Init(proposalId + 1, this);
        learner_->Init(this);
    }

    void PaxController::TryNewValue()
    {
        if (learner_->AnymoreToLearn())
        {
            // not catching up
            return;
        }

        if (!pax_config_->ProposingAuthority())
        {
            // not authorized
            return;
        }

        proposer_->NewValue();
    }

    void PaxController::PushSMByState(instance_id_t target_instance_id)
    {
        instance_id_t instanceInSM = state_machine_->GetSMInstanceId();
        for (instance_id_t i = instanceInSM; i < target_instance_id; ++i)
        {
            PaxosLogEntry paxos_state = acceptor_->ReadState(i);
            state_machine_->Execute(i, paxos_state.accepted_value());
        }
    }

    void PaxController::HandleMessage(const PaxMessage &message)
    {
        MessageType messageType = message.GetMessageType();
        if (MessagePlaceHolder(messageType))
        {
            // placeholder handle if necessary
            return;
        }
        else if (MessageForLearner(messageType))
        {
            learner_->HandleMessage(message);
        }
        else if (MessageForProposer(messageType))
        {
            proposer_->HandleMessage(message);
        }
        else if (MessageForAcceptor(messageType))
        {
            acceptor_->HandleMessage(message);
        }

        if (learner_->Learned())
        {
            // todo II: state machine execute
            state_machine_->Execute(learner_->GetInstanceId(), learner_->GetLearnedValue());
            InstanceDone();
            NewInstance();
        }
    }

    [[noreturn]] void *PaxController::MainLoop(void *)
    {
        while (true)
        {

            Event eventToHandle;
            while (Scheduler::SingleInstance()->Dispatch(eventToHandle))
            {
                eventToHandle();
            }
            EventTimeStamp nextEventTime;
            PaxMessage *paxMessage = nullptr;
            if (Scheduler::SingleInstance()->NextEventTime(nextEventTime))
            {
                if (msgProv_->Take(paxMessage, Time::DurationMS(STEADY_TIME_NOW, nextEventTime)))
                {
                    HandleMessage(*paxMessage);
                    delete paxMessage;
                }
            }
            else if (msgProv_->Take(paxMessage, Time::MS(500)))
            {
                // waiting 500 ms by default
                HandleMessage(*paxMessage);
                delete paxMessage;
            }

            TryNewValue();
        }
    }

    PaxController::~PaxController()
    {
        delete learner_;
        learner_ = nullptr;

        delete proposer_;
        proposer_ = nullptr;

        delete acceptor_;
        acceptor_ = nullptr;

        delete msgProv_;
        msgProv_ = nullptr;
    }

    void PaxController::InstanceDone()
    {
        proposer_->InstanceDone(instance_id_, learner_->GetLearnedValue());
    }

    void PaxController::NewInstance()
    {
        instance_id_++;
        acceptor_->NewInstance();
        proposer_->NewInstance();
        learner_->NewInstance();
    }

    void PaxController::AddMessage(PaxMessage &pax_message)
    {
        auto message = new PaxMessage(std::move(pax_message));
        msgProv_->TryAdd(message);
    }

    std::future<void *> PaxController::prov_loop;
}