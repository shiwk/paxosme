//
// Created by shiwk on 2020/7/18.
//

#include <proposer_pax.hpp>

namespace paxosme {

    /**
     *  Found a new value
     *  @param log_value
     */
    void PaxProposer::ProposeNew(LogValue &log_value) {
        if (proposer_state_->GetLogValue().empty()) {
            proposer_state_->SetLogValue(log_value);
        }
    }

    /**
     * prepare the new value wrapped in message.
     */
    void PaxProposer::Prepare() {
        proposer_status_ = ProposerStatus::kPrepare;
        auto pax_message = GenerateMessage(MessageType::kPrepareBroadCast);
        instance_id_t instanceId = pax_message.GetInstanceId();
        BroadCastMessage(pax_message);
        event_callback callback = [this, instanceId] { Prepare_Timeout_Callback(instanceId); };
        AddTimer(EventType::kPrepare, callback, prepare_delay_);
    }

    /**
     * Callback if prepare not accepted before timout
     * @param instanceId
     */
    void PaxProposer::Prepare_Timeout_Callback(instance_id_t instanceId) {
        if (instanceId != GetInstanceId()) {
            return; // timeout id inconsistent with instance id
        }

        // prepare again
        Prepare();
    }


    /**
     * Process prepare reply from other acceptors
     * @param pax_reply_message
     */
    void PaxProposer::HandlePrepareResponse(const PaxMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPrepare)
            return; // incompatible proposer status

        if (pax_reply_message.GetProposer() != GetNodeId())
            // reply not for me
            return;
        if (pax_reply_message.GetProposalId() != proposer_state_->GetMyProposal())
            return; // something goes wrong!

        if (!pax_reply_message.IsRejected()) {
            // count for approval

            TryUpdateProposerStateWithPrepareReply(pax_reply_message);
            pax_decider_->AddApproval(proposer_state_->GetMyProposal(), GetNodeId());

        } else {
            // reject and record proposal id promised by the replier
            proposer_state_->TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                        pax_reply_message.GetGeneratorId());
            pax_decider_->AddRejection(proposer_state_->GetMyProposal(), GetNodeId());
        }

        if (pax_decider_->IsMajorityAccepted()) {
            // got majority pre-accept
            Propose();
        } else if (pax_decider_->IsMajorityRejected() || !pax_decider_->IsStillPending()) {
            // re-launch prepare
            event_callback callback = [this] { Prepare(); };
            AddTimer(EventType::kPrepare, callback, prepare_delay_);
        }
    }


    /**
     * Propose value once prepare finished.
     */
    void PaxProposer::Propose() {
        proposer_status_ = ProposerStatus::kPropose;
        PaxMessage pax_message = GenerateMessage(MessageType::kProposeBroadCast);
        pax_decider_->Reset(); // reset for propose stage counter before broadcast
        BroadCastMessage(pax_message);
        instance_id_t instanceId = pax_message.GetInstanceId();
        event_callback callback = [this, instanceId] { Propose_Timeout_Callback(instanceId); };
        AddTimer(EventType::kPrepare, callback, prepare_delay_);
    }

    /**
     * Callback if propose not accepted before timeout.
     */
    void PaxProposer::Propose_Timeout_Callback(instance_id_t instanceId) {
        if (instanceId != GetInstanceId()) {
            return; // timeout id inconsistent with instance id
        }

        // prepare again
        Prepare();
    }

    /**
     *
     * @param pax_reply_message
     */
    void PaxProposer::HandleProposeResponse(const PaxMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPropose)
            return; // incompatible proposer status

        if (pax_reply_message.GetProposer() != GetNodeId())
            // reply not for me
            return;
        if (pax_reply_message.GetProposalId() != proposer_state_->GetMyProposal())
            return; // something goes wrong!

        if (!pax_reply_message.IsRejected()) {
            // count for approval
            pax_decider_->AddApproval(proposer_state_->GetMyProposal(), GetNodeId());
        } else {
            // reject and record proposal id promised by the replier
            proposer_state_->TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                        pax_reply_message.GetGeneratorId());
            pax_decider_->AddRejection(proposer_state_->GetMyProposal(), GetNodeId());
        }

        if (pax_decider_->IsMajorityAccepted()) {
            proposer_status_ = ProposerStatus::kMajorityAccepted;
            PaxMessage msg(GetNodeId(), MessageType::kSenderPublishChosenValue);
            // no log_value in msg
            msg.SetInstanceId(pax_reply_message.GetInstanceId());
            msg.SetProposalId(pax_reply_message.GetProposalId());

            ProcessChosenValue(msg);
            BroadCastMessage(msg);
        } else if (pax_decider_->IsMajorityRejected() || !pax_decider_->IsStillPending()) {
            event_callback callback = [this] { Prepare(); };
            AddTimer(EventType::kPrepare, callback, prepare_delay_);
        }
    }

    bool PaxProposer::TryUpdateProposerStateWithPrepareReply(const PaxMessage &message) {
        return proposer_state_->TryUpdateLogValue(message.GetProposalId(), message.GetProposer(),
                                                  message.GetAcceptedValue());
    }

    PaxMessage PaxProposer::GenerateMessage(MessageType message_type) {
        PaxMessage message(GetNodeId(), message_type);
        message.SetInstanceId(GetInstanceId());
        message.SetProposer(GetNodeId());
        message.SetProposer(proposer_state_->GetNewProposalId());
        message.SetProposedLogValue(proposer_state_->GetLogValue());
        return message;
    }

    void PaxProposer::Init() {

    }
}