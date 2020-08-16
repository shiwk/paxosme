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
        proposer_status_ = PaxProposerStatus::kPrePropose;
        auto pax_message = GeneratePreMessage();
        SetPaxMessage(pax_message);
        LaunchPrePropose();
        SetLogValue(log_value);
    }

    /**
     * Process pre-propose reply from other acceptors
     * @param pax_reply_message
     */
    void PaxProposer::HandlePreReply(const PaxReplyMessage &pax_reply_message) {

        if (proposer_status_ != PaxProposerStatus::kPrePropose)
            return; // incompatible proposer status

        OnReceivedReply(pax_reply_message);

        if (pax_decider_->IsToBeAccepted()) {
            Propose();
            proposer_status_ = PaxProposerStatus::kPropose;
        } else if (pax_decider_->IsStillPending()) {
            // todo: handle unaccepted case
        }
    }

    /**
     * Propose value once pre-propose finished
     */
    void PaxProposer::Propose() {
        pax_decider_->Reset();
        proposer_status_ = PaxProposerStatus::kPropose;
        // todo : check whether need update proposal_id
        //UpdatePaxMessage();
        LaunchPropose();
    }

    /**
     *
     * @param pax_reply_message
     */
    void PaxProposer::HandleProposeReply(const PaxReplyMessage &pax_reply_message) {

        if (proposer_status_ != PaxProposerStatus::kPropose)
            return; // incompatible proposer status

        OnReceivedReply(pax_reply_message);

        if (pax_decider_->IsToBeAccepted()) {
            // value chosen
            OnChosenValue();
            proposer_status_ = PaxProposerStatus::kPropose;
        } else if (pax_decider_->IsStillPending()) {
            // todo: handle unaccepted case
        }
    }

    /**
     * Handle message already accepted by majority
     */
    void PaxProposer::OnChosenValue() {
        ProcessMessageAcceptedByMajority(proposed_message_);
        proposer_status_ = PaxProposerStatus::kNone;
    }

    void PaxProposer::LaunchPrePropose() {
        PaxRequest pax_request(proposed_message_, RequestType::PreProposeBroadCast);
        BroadCastToAcceptors(pax_request);
    }

    void PaxProposer::LaunchPropose() {
        PaxRequest pax_request(proposed_message_, RequestType::ProposeBroadCast);
        BroadCastToAcceptors(pax_request);
    }

    void PaxProposer::SetPaxMessage(const PaxMessage &message) {
        proposed_message_ = message;
    }

    void PaxProposer::SetLogValue(const LogValue &value) {
        proposed_message_.SetLogValue(value);
    }

    void PaxProposer::OnReceivedReply(const PaxReplyMessage &pax_reply_message) {
        if (pax_reply_message.GetProposerId() != GetNodeId())
            // reply not for the local node
            return;
        if (pax_reply_message.GetProposerId() != proposed_message_.GetProposalId())
            return; // something goes wrong!

        if (pax_reply_message.IsAccepted()) {
            // count for approval
            pax_decider_->AddApproval(proposed_message_.GetProposalId(), GetNodeId());
        } else {
            // reject and record proposal id promised by the replier
            pax_state_->UpdateMaximalProposalId(pax_reply_message.GetPromisedId());
            pax_decider_->AddRejection(proposed_message_.GetProposalId(), GetNodeId())
        }
    }
}