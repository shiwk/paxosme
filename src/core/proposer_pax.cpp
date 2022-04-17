//
// Created by shiwk on 2020/7/18.
//

#include <proposer_pax.hpp>

namespace paxosme {

    int PaxProposer::PROPOSE_TIMEOUT = PROPOSE_TIMEOUT_CONST;
    int PaxProposer::ACCEPT_TIMEOUT = ACCEPT_TIMEOUT_CONST;

    PaxProposer::PaxProposer(const PaxConfig *config, const PaxCommunicator *communicator, const Storage *storage,
                             const Schedule *schedule)
            : PaxPlayer(config, communicator, storage, schedule),
              proposal_counter_(config) {
        status_ = ProposerStatus::kNone;
    }

    /**
     *  Found a new value
     *  @param log_value
     */
    void PaxProposer::NewValue() {
        LogValue log_value;
        if (!proposal_prov_->Flush(log_value, GetInstanceId())) {
            // no more new value
            return;
        }

        EventHandler callback = [this] { NewValueTimeoutCallback(); };
        if (proposer_state_.GetLogValue().empty()) {
            proposer_state_.SetLogValue(log_value);
        }
        Publish(EventType::kEVENT_NEWVALUE_TIMEOUT, callback, proposal_prov_->GetNewValueTimeout());
        Propose();
    }

    /**
     * prepare the new value wrapped in message.
     */
    void PaxProposer::Propose() {
        status_ = ProposerStatus::kPropose;

        if (proposal_counter_.SomeoneReject())
            proposer_state_.NewProposalId();
        proposal_id_t proposalId = proposer_state_.GetMyProposalId();

        proposal_counter_.Reset();

        auto pax_message = GenerateMessage(MessageType::kMSG_PROPOSE_BROADCAST, proposalId);
        instance_id_t instanceId = pax_message.GetInstanceId();
        EventHandler callback = [this, instanceId] { ProposeTimeoutCallback(instanceId); };
        Publish(EventType::kEVENT_PROPOSE_TIMEOUT, callback, PROPOSE_TIMEOUT);

        BroadCastMessage(pax_message);
    }

    /**
     * Callback if prepare not accepted before timout
     * @param instanceId
     */
    void PaxProposer::ProposeTimeoutCallback(instance_id_t instanceId) {
        if (instanceId != GetInstanceId()) {
            return; // timeout id inconsistent with instance id
        }

        // prepare again
        Propose();
    }

    /**
     * Process prepare reply from other acceptors
     * @param pax_reply_message
     */
    void PaxProposer::HandleProposeAck(const PaxMessage &pax_reply_message) {

        if (status_ != ProposerStatus::kPropose)
            return; // incompatible proposer status

        if (pax_reply_message.GetProposingNodeId() != GetNodeId())
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
            proposer_state_.TryUpdateHighestProposalId(pax_reply_message.GetPromisedProposalId(),
                                                       pax_reply_message.GetPromisedNodeId());
            proposal_counter_.AddRejection(proposer_state_.GetMyProposalId(), GetNodeId());
        }

        if (proposal_counter_.IsMajorityPromised()) {
            // got majority pre-accept
            Propose();
        } else if (proposal_counter_.IsMajorityRejected() || !proposal_counter_.IsStillPending()) {
            // re-launch prepare
            instance_id_t instanceId = pax_reply_message.GetInstanceId();
            EventHandler callback = [this, instanceId] { ProposeTimeoutCallback(instanceId); };
            Publish(EventType::kEVENT_PROPOSE_TIMEOUT, callback, PROPOSE_TIMEOUT);
        }
    }


    /**
     * Propose value once prepare finished.
     */
    void PaxProposer::Accept() {
        status_ = ProposerStatus::kAccept;
        PaxMessage pax_message = GenerateMessage(MessageType::kMSG_ACCEPT_BROADCAST, proposer_state_.GetMyProposalId());

        proposal_counter_.Reset(); // reset for propose stage counter before broadcast
        BroadCastMessage(pax_message);
        instance_id_t instanceId = pax_message.GetInstanceId();
        EventHandler callback = [this, instanceId] { ProposeTimeoutCallback(instanceId); };
        Publish(EventType::kEVENT_ACCEPT_TIMEOUT, callback, ACCEPT_TIMEOUT);
    }

    /**
     *
     * @param message
     */
    void PaxProposer::HandleAcceptAck(const PaxMessage &message) {

        if (status_ != ProposerStatus::kAccept)
            return; // incompatible proposer status

        if (message.GetProposingNodeId() != GetNodeId())
            // reply not for me
            return;
        if (message.GetProposalId() != proposer_state_.GetMyProposalId())
            return; // something goes wrong!

        if (!message.IsRejected()) {
            // count for approval
            proposal_counter_.AddAccept(proposer_state_.GetMyProposalId(), GetNodeId());
        } else {
            // reject and record proposal id promised by the replier
            proposer_state_.TryUpdateHighestProposalId(message.GetPromisedProposalId(),
                                                       message.GetPromisedNodeId());
            proposal_counter_.AddRejection(proposer_state_.GetMyProposalId(), GetNodeId());
        }

        if (proposal_counter_.IsMajorityAccepted()) {
            status_ = ProposerStatus::kMajorityAccepted;
            PaxMessage msg(GetNodeId(), MessageType::kMSG_VALUE_CHOSEN);
            // no log_value in msg
            msg.SetInstanceId(message.GetInstanceId());
            msg.SetProposalId(message.GetProposalId());

            ProcessChosenValue(msg);
//            BroadCastMessage(msg);
        } else if (proposal_counter_.IsMajorityRejected() || !proposal_counter_.IsStillPending()) {
            instance_id_t instanceId = message.GetInstanceId();
            EventHandler callback = [this, instanceId] { ProposeTimeoutCallback(instanceId); };
            Publish(EventType::kEVENT_ACCEPT_TIMEOUT, callback, ACCEPT_TIMEOUT);
        }
    }

    bool PaxProposer::TryUpdateProposerStateWithPrepareReply(const PaxMessage &message) {
        return proposer_state_.TryUpdateLogValue(message.GetProposalId(), message.GetProposingNodeId(),
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

        status_ = ProposerStatus::kNone;
    }

    void PaxProposer::InstanceDone(instance_id_t instance_id, const LogValue &log_value) {
        proposal_prov_->WriteResult(log_value, instance_id, kValueAccepted);
        Withdraw(kEVENT_PROPOSE_TIMEOUT);
        Withdraw(kEVENT_ACCEPT_TIMEOUT);
    }

    void PaxProposer::NewValueTimeoutCallback() {
        if (status_ != ProposerStatus::kPropose && status_ != ProposerStatus::kAccept){
            // this should not happen
            return ;
        }

        // exist proposing
        Withdraw(kEVENT_PROPOSE_TIMEOUT);
        Withdraw(kEVENT_ACCEPT_TIMEOUT);

        status_ = ProposerStatus::kNone;

        // flush pending value
        proposal_prov_->WriteResult("", GetInstanceId(), kValueTimeout);
    }

    void PaxProposer::HandleMessage(const PaxMessage &message) {
        switch (message.GetMessageType()) {

            // todo I: check instance and handle the case if the instance not matched

            case kMSG_PROPOSE_ACK: {
                HandleProposeAck(message);
                break;
            }
            case kMSG_ACCEPT_ACK:
                HandleAcceptAck(message);
                break;
            default:
                break;
        }
    }
}