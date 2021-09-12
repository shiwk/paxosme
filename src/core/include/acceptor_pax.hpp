//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_ACCEPTOR_PAX_HPP
#define PAXOSME_ACCEPTOR_PAX_HPP

#include <utility>

#include "player_pax.hpp"
#include "config_pax.hpp"

namespace paxosme {
    class AcceptorState {
    public:
        AcceptorState(LogValue  logValue) : accepted_value_(std::move(logValue)){}
        instance_id_t GetInstanceId() const;

        void SetInstanceId(instance_id_t instance_id);
            void init(const PaxosState &state);

        proposal_id_t GetPromisedProposal() const {
            return promised_id_;
        }

        node_id_t GetPromisedProposer() const {
            return promised_node_id_;
        }

        void SetPromisedProposal(proposal_id_t proposal_id, node_id_t proposer_id) {
            promised_id_ = proposal_id;
            promised_node_id_ = proposer_id;
        }

        void SetAcceptedProposal(proposal_id_t proposal_id, node_id_t proposer_id) {
            accepted_proposal_id_ = proposer_id;
            accepted_proposer_ = proposer_id;
        }

        proposal_id_t GetAcceptedProposalId() const {
            return accepted_proposal_id_;
        }

        node_id_t GetAcceptedNodeId() const {
            return accepted_proposer_;
        }

        void SetAcceptedValue(const LogValue &accepted_value) {
            accepted_value_ = accepted_value;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }



    private:
        proposal_id_t promised_id_;
        node_id_t promised_node_id_;
        instance_id_t instance_id_;
        proposal_id_t accepted_proposal_id_;
        node_id_t accepted_proposer_;
        LogValue accepted_value_;
    };

    class PaxAcceptorReplyMessage {
        proposal_id_t accepted_id_;
        LogValue accepted_value_;
        proposal_id_t promised_id_;
        node_id_t promised_node_id_;
        instance_id_t instance_id_;

    private:
        bool is_rejected_;
        node_id_t proposer_id_;

    public:
        node_id_t GetPromisedNodeId() const {
            return promised_node_id_;
        }

        instance_id_t GetInstanceId() const {
            return instance_id_;
        }

        void SetInstanceId(instance_id_t instance_id) {
            instance_id_ = instance_id;
        }

        void SetIsRejected(bool is_rejected) {
            is_rejected_ = is_rejected;
        }

        node_id_t GetProposerId() const {
            return proposer_id_;
        }

        proposal_id_t GetAcceptedId() const {
            return accepted_id_;
        }

        proposal_id_t GetPromisedId() const {
            return promised_id_;
        }

        bool IsRejected() const {
            return is_rejected_;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }

        void SetProposerId(node_id_t proposer_id) {
            proposer_id_ = proposer_id;
        }

        void SetAcceptedId(proposal_id_t accepted_id) {
            accepted_id_ = accepted_id;
        }

        void SetAcceptedValue(const LogValue &accepted_value) {
            accepted_value_ = accepted_value;
        }

        void SetPromisedId(proposal_id_t promised_id) {
            promised_id_ = promised_id;
        }

        void SetPromisedNodeId(node_id_t promised_node_id) {
            promised_node_id_ = promised_node_id;
        }
    };

    class PaxAcceptor : public PaxPlayer {
        AcceptorState *acceptor_state_;
    public:
        void HandlePrepareRequest(const PaxMessage &message);

        void HandleProposeRequest(const PaxMessage &message);

        void ReplyProposer(const PaxAcceptorReplyMessage &reply, MessageType request_type);

        bool IsAccepted();

        bool IsHigherThanPromised(node_id_t, proposal_id_t);

        void UpdatePromised(node_id_t, proposal_id_t);

        const LogValue &GetAcceptedValue();

        proposal_id_t GetAcceptedProposalId();

        node_id_t GetAcceptedNodeId();

        instance_id_t Init();

        bool HandleSenderPublish(const PaxMessage &message);

        bool IsProposalAccepted(proposal_id_t proposal_id, node_id_t node_id);
    };


}

#endif //PAXOSME_ACCEPTOR_PAX_HPP
