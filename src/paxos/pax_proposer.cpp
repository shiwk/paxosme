//
// Created by shiwk on 2020/7/18.
//

#include <pax_proposer.hpp>

namespace paxosme {

    /**
     *  Propose a new value (trigger a new round)
     *  @param log_value
     */
    void PaxProposer::ProposeNew(const LogValue &log_value) {
        pax_decider_->Reset();
        PrePropose(log_value);
    }

    /**
     * Pre-Propose the new value wrapped in message.
     * @param log_value
     */
    void PaxProposer::PrePropose(const LogValue &log_value) {
        proposer_status_ = ProposerStatus::kPrePropose;
        auto pax_message = GeneratePreMessage();
        SetPaxMessage(pax_message);
        LaunchPrePropose();
        UpdateLogValue(log_value);
    }

    /**
     * Process pre-propose reply from other acceptors
     * @param pax_reply_message
     */
    void PaxProposer::HandlePreProposeResponse(const PaxAcceptorReplyMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPrePropose)
            return; // incompatible proposer status

        HandleReceivedReply(pax_reply_message);

        if (pax_decider_->IsMajorityAccepted()) {
            Propose();
        } else if (!pax_decider_->IsStillPending()) {
            if (pax_decider_->IsMajorityRejected()) {
                HandleAbandonValue();
            }
            // todo: another proposing to be prepared since the proposal is not pending anymore.
        }
    }

    /**
     * Propose value once pre-propose finished
     */
    void PaxProposer::Propose() {
        pax_decider_->Reset(); // reset for propose stage counter
        // todo : check whether need update proposal_id
        LaunchPropose();
    }

    /**
     *
     * @param pax_reply_message
     */
    void PaxProposer::HandleProposeResponse(const PaxAcceptorReplyMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPropose)
            return; // incompatible proposer status

        HandleReceivedReply(pax_reply_message);

        if (pax_decider_->IsMajorityAccepted()) {
            HandleChosenValue(); // value chosen
        } else {
            if (pax_decider_->IsMajorityRejected()) {
                HandleAbandonValue();
            }
            // todo: another proposing to be prepared since the proposal is not pending anymore.
        }
    }

    /**
     * Handle message already accepted by majority
     */
    void PaxProposer::HandleChosenValue() {
        proposer_status_ = ProposerStatus::kMajorityAccepted;
        PaxMessage pax_message = proposer_state_->GetPendingMessage();
        ProcessAcceptedMessage(pax_message);
    }

    void PaxProposer::HandleAbandonValue() {
        proposer_status_ = ProposerStatus::kMajorityRejected;
        // todo
    }

    void PaxProposer::LaunchPrePropose() {
        PaxMessage pax_message = proposer_state_->GetPendingMessage();
        BroadCast(pax_message, MessageType::PreProposeBroadCast);
    }

    void PaxProposer::LaunchPropose() {
        proposer_status_ = ProposerStatus::kPropose;
        PaxMessage pax_message = proposer_state_->GetPendingMessage();
        BroadCast(pax_message, MessageType::ProposeBroadCast);
    }

    void PaxProposer::SetPaxMessage(const PaxMessage &message) {
        proposer_state_->SetPendingMessage(message);
    }

    void PaxProposer::UpdateLogValue(const LogValue &value) {
        proposer_state_->SetLogValue(value);
    }

    void PaxProposer::HandleReceivedReply(const PaxAcceptorReplyMessage &pax_reply_message) {
        if (pax_reply_message.GetProposerId() != GetNodeId())
            // reply not for the local node
            return;
        if (pax_reply_message.GetProposerId() != proposer_state_->GetPendingProposalId())
            return; // something goes wrong!

        if (!pax_reply_message.IsRejected()) {
            // count for approval
            pax_decider_->AddApproval(proposer_state_->GetPendingProposalId(), GetNodeId());

            // try to update log_value if
            if (proposer_status_ & 0x03)
                TryUpdateProposerStateWithAcceptorReply(pax_reply_message);

        } else {
            // reject and record proposal id promised by the replier
            proposer_state_->TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                        pax_reply_message.GetReplierId());
            pax_decider_->AddRejection(proposer_state_->GetPendingProposalId(), GetNodeId());
        }
    }

    bool PaxProposer::TryUpdateProposerStateWithAcceptorReply(const PaxAcceptorReplyMessage &message) {
        return proposer_state_->TryUpdateLogValue(message.GetProposerId(), message.GetProposerId(),
                                                  message.GetAcceptedValue());
    }

    PaxMessage PaxProposer::GeneratePreMessage() const {
        return {GetInstanceId(), proposer_state_->GetApplicableProposalId(), GetNodeId()};
    }
}