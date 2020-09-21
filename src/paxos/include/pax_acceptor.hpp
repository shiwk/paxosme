//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_ACCEPTOR_HPP
#define PAXOSME_PAX_ACCEPTOR_HPP

#include "pax_player.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class AcceptorState{
        proposal_id_t promised_id_;
        node_id_t promised_node_id_;

        proposal_id_t accepted_proposal_id_;
        node_id_t accepted_node_id_;

    private:
        LogValue accepted_value_;

    public:
        proposal_id_t GetPromisedId() const {
            return promised_id_;
        }

        node_id_t GetPromisedNodeId() const {
            return promised_node_id_;
        }

        void SetPromisedProposal(proposal_id_t proposal_id, node_id_t proposer_id){
            promised_id_ = proposal_id;
            promised_node_id_ = proposer_id;
        }

        void SetAcceptedProposal(proposal_id_t proposal_id, node_id_t proposer_id){
            accepted_proposal_id_ = proposer_id;
            accepted_node_id_ = proposer_id;
        }

        proposal_id_t GetAcceptedProposalId() const {
            return accepted_proposal_id_;
        }

        node_id_t GetAcceptedNodeId() const {
            return accepted_node_id_;
        }

        void SetAcceptedValue(const LogValue &accepted_value) {
            accepted_value_ = accepted_value;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }

    };

    class PaxAcceptor : public PaxPlayer{
        AcceptorState *acceptor_state_;
    public:
        void HandlePreProposeRequest(const PaxMessage& message);

        void HandleProposeRequest(const PaxMessage& message);

        void ReplyProposer(const PaxAcceptorReplyMessage& pax_acceptor_reply_message, node_id_t proposer_id, RequestType request_type);

        bool IsAccepted();
    };
}

#endif //PAXOSME_PAX_ACCEPTOR_HPP
