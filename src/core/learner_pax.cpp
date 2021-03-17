//
// Created by shiwk on 2020/7/18.
//

#include <learner_pax.hpp>
#include <controller.hpp>

namespace paxosme {
    const LogValue &PaxLearner::GetLearnedValue() {
        return learner_state_->GetLearnedValue();
    }

    void PaxLearner::LearnFromOthers(const PaxMessage &pax_message) {
        instance_id_t instance_id = pax_message.GetInstanceId();
        if (instance_id != GetInstanceId())
            return;

        // todo I: check already learned or not
        // persist needed since learned from other learners
        LearnNew(pax_message.GetProposedLogValue(), pax_message.GetInstanceId(),
                                 pax_message.GetProposalId(),
                                 pax_message.GetProposer(), true);
    }

    void PaxLearner::RequestLearning() {
        node_id_t node_id = GetNodeId();
        instance_id_t instance_id = GetInstanceId();

        PaxMessage message(node_id, MessageType::kLearnerNewRequest);
        message.SetInstanceId(instance_id);
        BroadCastMessage(message);
    }

    void PaxLearner::HandleLearningRequest(const NewValueRequest &new_value_request) {
        SetPossibleHigherInstanceId(new_value_request.GetInstanceId());

        // todo I: checke state is confirmed
        // todo II: be careful race condition for learner state
        if (new_value_request.GetInstanceId() + 1 != GetInstanceId()) {
            // instance not matched
            // tell my instance id
            TellInstanceId(GetInstanceId(), new_value_request.GetNodeId());
            return;
        }

        ReplyLearning(new_value_request.GetNodeId());
    }

    void PaxLearner::SetPossibleHigherInstanceId(const instance_id_t &instance_id) {
        if (instance_id <= highest_known_instance_id_)
            return;
        highest_known_instance_id_ = instance_id;
    }

    void PaxLearner::HandleTellNewInstanceId(const instance_id_t instance_id, const node_id_t node_id) {
        SetPossibleHigherInstanceId(instance_id);
        // todo II : checkpoint update needed
    }

//    void PaxLearner::HandleReplyLearning(const PaxMessage &pax_message) {
//        if (pax_message.GetInstanceId() != GetInstanceId()) {
//            // instance not match
//            return;
//        }
//
//        // learn new value
//        learner_state_->LearnNew(pax_message.GetProposedLogValue(), pax_message.GetInstanceId(),
//                                 pax_message.GetProposalId(),
//                                 pax_message.GetProposer());
//    }

    void PaxLearner::TellInstanceId(const instance_id_t instance_id, const node_id_t node_id) {
        PaxMessage message(GetNodeId(), MessageType::kTellInstanceId);
        message.SetInstanceId(GetInstanceId());
        // todo II : set confirmed instance id and whether checkpoint needed
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

    void PaxLearner::LearnNew(const LogValue &value, instance_id_t instance_id, proposal_id_t proposal_id, node_id_t proposer, bool writeState) {
        // todo I::
        learner_state_->LearnNew(value, instance_id, proposal_id, proposer);
        Persist(value, instance_id, proposal_id, proposer);
    }

//    void PaxLearner::ReplyLearning(instance_id_t instance_id) {
//        //todo I : read from storage
//        PaxMessage message(GetNodeId(), MessageType::kLearnerNewReply);
//    }

}