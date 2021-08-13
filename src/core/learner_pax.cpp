//
// Created by shiwk on 2020/7/18.
//

#include <learner_pax.hpp>
#include <controller.hpp>

namespace paxosme {
    const LogValue &PaxLearner::GetLearnedValue() {
        return learner_state_->GetLearnedValue();
    }

    void PaxLearner::ShallILearn() {
        node_id_t node_id = GetNodeId();
        instance_id_t instance_id = GetInstanceId();

        PaxMessage message(node_id, MessageType::kShallILearn);
        message.SetInstanceId(instance_id);
        BroadCastMessage(message);
    }

    void PaxLearner::HandleLearningRequest(const NewValueRequest &new_value_request) {
        SetPossibleHigherInstanceId(new_value_request.GetInstanceId());

        // todo II: be careful race condition for learner state
        if (new_value_request.GetInstanceId() < learner_state_->GetLearnedInstanceId() // learner is much lower than me
            || new_value_request.GetInstanceId() + 1 != GetInstanceId()// or learner is not catching me
                )
        {

            TellInstanceId(GetInstanceId(), new_value_request.GetNodeId());
            return;
        }

        if (new_value_request.GetInstanceId() + 1 == GetInstanceId()) {
            //  learner is just following me, and in this case return the value directly.
            SendLearnedValue(new_value_request.GetInstanceId(), new_value_request.GetNodeId());
        }
    }

    void PaxLearner::SetPossibleHigherInstanceId(const instance_id_t &instance_id) {
        if (instance_id <= highest_known_instance_id_)
            return;
        highest_known_instance_id_ = instance_id;
    }

    void PaxLearner::HandleTellNewInstanceId(const PaxMessage& pax_message) {
        SetPossibleHigherInstanceId(pax_message.GetLeaderInstanceId());

        if (pax_message.GetInstanceId() != GetInstanceId()) {
            // ignore message because out of date
            return;
        }

        if (pax_message.GetLeaderInstanceId() < GetInstanceId()) {
            // ignore because it does not lead on me
            return;
        }
        // todo II : compare checkpoint and sync checkpoint if needed

        if (!is_learning_)
            ConfirmLearn(pax_message.GetSenderId());
    }

    void PaxLearner::TellInstanceId(const instance_id_t instance_id, const node_id_t node_id) {
        PaxMessage message(GetNodeId(), MessageType::kTellInstanceId);
        message.SetInstanceId(instance_id);
        message.SetLeaderInstanceId(GetInstanceId());

        // todo II : should send my checkpoint if much lower than me.
        SendMessage(message, node_id);
    }

    void PaxLearner::TellOtherLearners() {
        PaxMessage message(GetNodeId(), MessageType::kChosen);
        message.SetInstanceId(GetInstanceId());
        message.SetProposalId(controller_->GetAcceptedProposal());
        message.SetProposer(controller_->GetAcceptedNodeId());


        BroadCastMessage(message);
    }

    void PaxLearner::LearnBySelf(const PaxMessage &pax_message) {
        if (pax_message.GetInstanceId() != GetInstanceId())
            return; // instance id not matched
        if (pax_message.GetProposer() != GetNodeId())
            return; // proposer not matched

        proposal_id_t proposal = controller_->GetAcceptedProposal();
        if (proposal == 0 || pax_message.GetProposalId() != proposal)
            return; // proposal == 0 means not accepted yet

        node_id_t node_id = controller_->GetAcceptedNodeId();
        if (node_id != GetNodeId())
            return;

        const LogValue &log_value = controller_->GetAcceptedValue();

        // no need to persist since acceptor should have done
        LearnNew(log_value, pax_message.GetInstanceId(), proposal, node_id, false);

        TellOtherLearners(); // tell others
    }

    bool PaxLearner::AnymoreToLearn() {
        return GetInstanceId() + 1 < highest_known_instance_id_;
    }

    void PaxLearner::LearnNew(const LogValue &value, instance_id_t instance_id, proposal_id_t proposal_id,
                              node_id_t proposer, bool writeState) {
        learner_state_->LearnNew(value, instance_id, proposal_id, proposer);
        if (writeState) {
            PaxosState paxos_state;
            paxos_state.set_promised_proposal_id(proposal_id);
            paxos_state.set_promised_node_id(proposer);
            paxos_state.set_instance_id(instance_id);
            paxos_state.set_accepted_proposal_id(proposal_id);
            paxos_state.set_accepted_node_id(proposer);
            paxos_state.set_accepted_value(value.GetValue());
            WriteState(paxos_state);
        }
    }


    void PaxLearner::SendLearnedValue(instance_id_t instanceId, node_id_t toNodeId) {
        PaxosState paxosState = ReadState(instanceId);

        PaxMessage pax_message(toNodeId, MessageType::kSendValue);
        pax_message.SetInstanceId(instanceId);
        pax_message.SetLearnedValue(LogValue(paxosState.accepted_value()));
        pax_message.SetAcceptedId(paxosState.accepted_proposal_id());
        pax_message.SetProposer(paxosState.proposer_id());
        SendMessage(pax_message, toNodeId);
    }

    void PaxLearner::ConfirmLearn(node_id_t node_id) {
        is_learning_ = true;
        PaxMessage pax_message(node_id, MessageType::kSendValue);
        pax_message.SetInstanceId(GetInstanceId());
    }

    void PaxLearner::HandleConfirmLearn(PaxMessage pax_message) {
        // todo I : send data to my learner
    }
}