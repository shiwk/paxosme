//
// Created by shiwk on 2020/7/18.
//

#include <proposer_pax.hpp>

namespace paxosme {

    int PaxProposer::PREPARE_TIMEOUT = PREPARE_TIMEOUT_CONST;
    int PaxProposer::PROPOSE_TIMEOUT = PROPOSE_TIMEOUT_CONST;

    PaxProposer::PaxProposer(const PaxConfig *config, const PaxCommunicator *communicator, const Storage *storage,
                             const Schedule *schedule)
                             : PaxPlayer(config, communicator, storage, schedule),
                             proposal_counter_(config) {
        proposer_status_ = ProposerStatus::kNone;
    }

    /**
     *  Found a new value
     *  @param log_value
     */
    void PaxProposer::ProposeNew(LogValue &log_value) {
        if (proposer_state_.GetLogValue().empty()) {
            proposer_state_.SetLogValue(log_value);
        }
    }

    /**
     * prepare the new value wrapped in message.
     */
    void PaxProposer::Prepare(bool newPrepare) {
        proposer_status_ = ProposerStatus::kPrepare;
        proposal_id_t proposalId = newPrepare ? proposer_state_.NewProposalId() : proposer_state_.GetMyProposalId();
        auto pax_message = GenerateMessage(MessageType::kPrepareBroadCast, proposalId);
        instance_id_t instanceId = pax_message.GetInstanceId();
        BroadCastMessage(pax_message);
        EventHandler callback = [this, instanceId] { ProposerTimeoutCallback(instanceId, false); };
        Publish(EventType::kPrepareTimeout, callback, PREPARE_TIMEOUT);
    }

    /**
     * Callback if prepare not accepted before timout
     * @param instanceId
     */
    void PaxProposer::ProposerTimeoutCallback(instance_id_t instanceId, bool needNewPrepare) {
        if (instanceId != GetInstanceId()) {
            return; // timeout id inconsistent with instance id
        }

        // prepare again
        Prepare(needNewPrepare);
    }

    /**
     * Process prepare reply from other acceptors
     * @param pax_reply_message
     */
    void PaxProposer::HandlePrepareReply(const PaxMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPrepare)
            return; // incompatible proposer status

        if (pax_reply_message.GetProposer() != GetNodeId())
            // reply not for me
            return;
        if (pax_reply_message.GetProposalId() != proposer_state_.GetMyProposalId())
            return; // something goes wrong!

        if (!pax_reply_message.IsRejected()) {
            // count for approval

            TryUpdateProposerStateWithPrepareReply(pax_reply_message);
            proposal_counter_.AddPromise(proposer_state_.GetMyProposalId(), GetNodeId());

        } else {
            // reject and record proposal id promised by the replier
            proposer_state_.TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                       pax_reply_message.GetPromisedNodeId());
            proposal_counter_.AddRejection(proposer_state_.GetMyProposalId(), GetNodeId());
        }

        if (proposal_counter_.IsMajorityPromised()) {
            // got majority pre-accept
            Propose();
        } else if (proposal_counter_.IsMajorityRejected() || !proposal_counter_.IsStillPending()) {
            // re-launch prepare
            instance_id_t instanceId = pax_reply_message.GetInstanceId();
            EventHandler callback = [this, instanceId] { ProposerTimeoutCallback(instanceId, true); };
            Publish(EventType::kPrepareTimeout, callback, PREPARE_TIMEOUT);
        }
    }


    /**
     * Propose value once prepare finished.
     */
    void PaxProposer::Propose() {
        proposer_status_ = ProposerStatus::kPropose;
        PaxMessage pax_message = GenerateMessage(MessageType::kProposeBroadCast, proposer_state_.GetMyProposalId());
        proposal_counter_.Reset(); // reset for propose stage counter before broadcast
        BroadCastMessage(pax_message);
        instance_id_t instanceId = pax_message.GetInstanceId();
        EventHandler callback = [this, instanceId] { ProposerTimeoutCallback(instanceId, false); };
        Publish(EventType::kProposeTimeout, callback, PROPOSE_TIMEOUT);
    }

    /**
     *
     * @param pax_reply_message
     */
    void PaxProposer::HandleProposeReply(const PaxMessage &pax_reply_message) {

        if (proposer_status_ != ProposerStatus::kPropose)
            return; // incompatible proposer status

        if (pax_reply_message.GetProposer() != GetNodeId())
            // reply not for me
            return;
        if (pax_reply_message.GetProposalId() != proposer_state_.GetMyProposalId())
            return; // something goes wrong!

        if (!pax_reply_message.IsRejected()) {
            // count for approval
            proposal_counter_.AddAccept(proposer_state_.GetMyProposalId(), GetNodeId());
        } else {
            // reject and record proposal id promised by the replier
            proposer_state_.TryUpdateHighestProposalId(pax_reply_message.GetPromisedId(),
                                                       pax_reply_message.GetPromisedNodeId());
            proposal_counter_.AddRejection(proposer_state_.GetMyProposalId(), GetNodeId());
        }

        if (proposal_counter_.IsMajorityAccepted()) {
            proposer_status_ = ProposerStatus::kMajorityAccepted;
            PaxMessage msg(GetNodeId(), MessageType::kSenderPublishChosenValue);
            // no log_value in msg
            msg.SetInstanceId(pax_reply_message.GetInstanceId());
            msg.SetProposalId(pax_reply_message.GetProposalId());

            ProcessChosenValue(msg);
            BroadCastMessage(msg);
        } else if (proposal_counter_.IsMajorityRejected() || !proposal_counter_.IsStillPending()) {
            instance_id_t instanceId = pax_reply_message.GetInstanceId();
            EventHandler callback = [this, instanceId] { ProposerTimeoutCallback(instanceId, true); };
            Publish(EventType::kProposeTimeout, callback, PROPOSE_TIMEOUT);
        }
    }

    bool PaxProposer::TryUpdateProposerStateWithPrepareReply(const PaxMessage &message) {
        return proposer_state_.TryUpdateLogValue(message.GetProposalId(), message.GetProposer(),
                                                 message.GetAcceptedValue());
    }

    PaxMessage PaxProposer::GenerateMessage(MessageType message_type, proposal_id_t proposal_id) {
        PaxMessage message(GetNodeId(), message_type);
        message.SetInstanceId(GetInstanceId());
        message.SetProposer(GetNodeId());
        message.SetProposer(proposal_id);
        message.SetProposedLogValue(proposer_state_.GetLogValue());
        return message;
    }

    void PaxProposer::Init(proposal_id_t proposal_id, const PaxController *controller) {
        PaxPlayer::InitController(controller);
        proposer_state_.Init(proposal_id);
    }

    void PaxProposer::NewInstance() {
        proposal_counter_.Reset();
        proposer_state_.Reset();
        proposer_status_ = ProposerStatus::kNone;
    }
}