//
// Created by shiwk on 2020/7/18.
//


#include <pax_proposer.hpp>

namespace paxosme {

    /**
     *  Propose a new value wrapped in <PaxMessage>
     *  @param log_value
     */
    void PaxProposer::Propose(const LogValue &log_value) {
        pax_decider_->Reset();
        PrePropose(log_value);
    }

    /**
     * Pre-Propose the new value wrapped in message.
     * @param log_value
     */
    void PaxProposer::PrePropose(const LogValue &log_value) {
        proposer_status_ = PaxProposerStatus::kPrepare;
        auto pax_message = GenerateMessage(log_value);
        BroadCast(pax_message);
    }

    /**
     * Process prepare reply from other acceptors
     * @param pre_reply_message
     */
    void PaxProposer::HandlePreReply(PaxPreReplyMessage &pre_reply_message) {
        if (pre_reply_message.GetProposerId() != GetNodeId())
            // reply not for the local node
            return;
        if (proposer_status_ != PaxProposerStatus::kPrepare)
            // incompatible proposer status
            return;

        if (pre_reply_message.GetProposerId() != proposed_message_->GetProposalId())
            return; // something goes wrong!

        if (pre_reply_message.IsAccepted()) {
            // count for approval
            pax_decider_->AddApproval(proposed_message_->GetProposalId(), GetNodeId());
        } else {
            // reject and record proposal id promised by the replier
            pax_state_->UpdateMaximalProposalId(pre_reply_message.GetPromisedId());
        }

        if (pax_decider_->IsToBeAccepted()) {
            // value chosen
            OnChosenValue();
        }
    }

    /**
     * Handle message already accepted by majority
     */
    void PaxProposer::OnChosenValue(LogValue &log_value) {
        proposer_status_ = PaxProposerStatus::kAccept;
        PaxMessage pax_message = GenerateMessage(log_value);
        ProcessMessageAcceptedByMajority(pax_message);
        BroadCast(pax_message);
    }

    void PaxProposer::BroadCast(const PaxMessage &message) {

    }
}