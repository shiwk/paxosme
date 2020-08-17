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
    void PaxProposer::HandlePreProposeResponse(const PaxReplyMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPrePropose)
            return; // incompatible proposer status

        OnReceivedReply(pax_reply_message);

        if (pax_decider_->IsToBeAccepted()) {
            Propose();
            proposer_status_ = ProposerStatus::kPropose;
        } else if (pax_decider_->IsStillPending()) {
            // todo: handle unaccepted case
        }
    }

    /**
     * Propose value once pre-propose finished
     */
    void PaxProposer::Propose() {
        pax_decider_->Reset();
        proposer_status_ = ProposerStatus::kPropose;
        // todo : check whether need update proposal_id
        //UpdatePaxMessage();
        LaunchPropose();
    }

    /**
     *
     * @param pax_reply_message
     */
    void PaxProposer::HandleProposeResponse(const PaxReplyMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPropose)
            return; // incompatible proposer status

        OnReceivedReply(pax_reply_message);

        if (pax_decider_->IsToBeAccepted()) {
            // value chosen
            OnChosenValue();
            proposer_status_ = ProposerStatus::kPropose;
        } else if (pax_decider_->IsStillPending()) {
            // todo: handle unaccepted case
        }
    }

    /**
     * Handle message already accepted by majority
     */
    void PaxProposer::OnChosenValue() {
        PaxMessage pax_message = proposer_state_->GetPendingMessage();
        ProcessMessageAcceptedByMajority(pax_message);
        proposer_status_ = ProposerStatus::kNone;
    }

    void PaxProposer::LaunchPrePropose() {
        PaxMessage pax_message = proposer_state_->GetPendingMessage();
        PaxRequest pax_request(pax_message, RequestType::PreProposeBroadCast);
        BroadCastToAcceptors(pax_request);
    }

    void PaxProposer::LaunchPropose() {
        PaxMessage pax_message = proposer_state_->GetPendingMessage();
        PaxRequest pax_request(pax_message, RequestType::ProposeBroadCast);
        BroadCastToAcceptors(pax_request);
    }

    void PaxProposer::SetPaxMessage(const PaxMessage &message) {
        proposer_state_->SetPendingMessage(message);
    }

    void PaxProposer::UpdateLogValue(const LogValue &value) {
        proposer_state_->SetLogValue(value);
    }

    void PaxProposer::OnReceivedReply(const PaxReplyMessage &pax_reply_message) {
        if (pax_reply_message.GetProposerId() != GetNodeId())
            // reply not for the local node
            return;
        if (pax_reply_message.GetProposerId() != proposer_state_->GetPendingProposalId())
            return; // something goes wrong!

        if (!pax_reply_message.IsRejected()) {
            // count for approval
            pax_decider_->AddApproval(proposer_state_->GetPendingProposalId(), GetNodeId());

            // update log_value
            if (proposer_status_ == ProposerStatus::kPrePropose)
                TryUpdatePaxMessageWithPreProposeReply(pax_reply_message);

        } else {
            // reject and record proposal id promised by the replier
            proposer_state_->TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                        pax_reply_message.GetReplierId());
            pax_decider_->AddRejection(proposer_state_->GetPendingProposalId(), GetNodeId());
        }
    }

    void PaxProposer::TryUpdatePaxMessageWithPreProposeReply(PaxReplyMessage message) {
        proposer_state_->TryUpdateLogValue(message.GetProposerId(), message.GetProposerId(),
                                           message.GetAcceptedValue());
    }
}