//
// Created by shiwk on 2020/7/18.
//

#include <proposer_pax.hpp>

namespace paxosme {

    /**
     *  Propose a new value (trigger a new round)
     *  @param log_value
     */
    void PaxProposer::ProposeNew() {
        pax_decider_->Reset();
        Prepare();
    }

    /**
     * prepare the new value wrapped in message.
     * @param log_value
     */
    void PaxProposer::Prepare() {
        proposer_status_ = ProposerStatus::kPrepare;
        auto pax_message = GenerateMessage(MessageType::kPrepareBroadCast);
        BroadCastMessage(pax_message);
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

            TryUpdateProposerStateWithAcceptorReply(pax_reply_message);
            pax_decider_->AddApproval(proposer_state_->GetMyProposal(), GetNodeId());

        } else {
            // reject and record proposal id promised by the replier
            proposer_state_->TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                        pax_reply_message.GetSelfId());
            pax_decider_->AddRejection(proposer_state_->GetMyProposal(), GetNodeId());
        }

        if (pax_decider_->IsMajorityAccepted()) {
            // got majority pre-accept
            Propose();
        } else if (pax_decider_->IsMajorityRejected() || !pax_decider_->IsStillPending()) {
            //todo I: handle prepare failed case, another proposing to be prepared since the proposal is not pending anymore.
            // todo: another proposing to be prepared since the proposal is not pending anymore.
        }
    }


/**
 * Propose value once prepare finished
 */
    void PaxProposer::Propose() {
        proposer_status_ = ProposerStatus::kPropose;
        // todo : check whether need update proposal_id
        PaxMessage pax_message = GenerateMessage(MessageType::kProposeBroadCast);
        pax_decider_->Reset(); // reset for propose stage counter before broadcast
        BroadCastMessage(pax_message);
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
                                                        pax_reply_message.GetSelfId());
            pax_decider_->AddRejection(proposer_state_->GetMyProposal(), GetNodeId());
        }

        if (pax_decider_->IsMajorityAccepted()) {
            proposer_status_ = ProposerStatus::kMajorityAccepted;
            PaxMessage msg(GetNodeId(), MessageType::kPublishChosenValue);
            // no log_value in msg
            msg.SetInstanceId(pax_reply_message.GetInstanceId());
            msg.SetProposalId(pax_reply_message.GetProposalId());

            ProcessChosenValue(msg);
            BroadCastMessage(msg);
        } else if (pax_decider_->IsMajorityRejected() || !pax_decider_->IsStillPending()) {
            // todo II: handle value abandoned
        }
    }

    bool PaxProposer::TryUpdateProposerStateWithAcceptorReply(const PaxMessage &message) {
        return proposer_state_->TryUpdateLogValue(message.GetProposalId(), message.GetProposer(),
                                                  message.GetAcceptedValue());
    }

    PaxMessage PaxProposer::GenerateMessage(MessageType message_type) {
        PaxMessage message(GetNodeId(), message_type);
        message.SetInstanceId(GetInstanceId());
        message.SetProposer(GetNodeId());
        message.SetProposer(proposer_state_->GetNewProposalId());
        message.SetProposedLogValue(*(proposer_state_->GetLogValue()));
        return message;
    }

}