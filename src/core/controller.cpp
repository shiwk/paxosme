//
// Created by shiwk on 2020/8/1.
//

#include <controller.hpp>

namespace paxosme {
    static void *thread_func(paxosme::PaxController *p) {
        p->FlushProv();
        return nullptr;
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

    void PaxController::Init(PaxConfig *pax_config) {
        pax_config_ = pax_config;
        instance_id_t instanceInState = acceptor_->Init();
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

        prov_loop_ = std::async(std::launch::async, thread_func, this);
        proposer_->Init();
        learner_->Init();
    }

    void PaxController::FlushProv() {
        while (true) {
            LogValue log_value;
            if (proposal_prov_->GetNewSubmit(instance_id_, log_value)) {
                proposer_->ProposeNew(log_value);
            }
        }
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
            case MessageType::kSenderPublishChosenValue: {
                if (!acceptor_->HandleSenderPublish(message)) {
                    // acceptor failed
                    return;
                }

                if (!learner_->HandleSenderPublish(message)) {
                    // learner failed
                    return;
                }
            }

            case MessageType::kBroadCastChosen : {
                learner_->HandleLeaderPublish(message);
            }

            // todo I: more cases
        }


        if (learner_->Learned()) {
            // todo I: new instance
        }
    }
}