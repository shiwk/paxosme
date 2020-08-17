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
        proposal_id_t accepted_id_;
        LogValue accepted_value_;

    public:
        proposal_id_t GetPromisedId() const {
            return promised_id_;
        }

        node_id_t GetPromisedNodeId() const {
            return promised_node_id_;
        }

        void SetPromisedProposal(proposal_id_t proposal_id, node_id_t node_id){
            promised_id_ = proposal_id;
            promised_node_id_ = node_id;
        }
        proposal_id_t GetAcceptedId() const {
            return accepted_id_;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }

    };

    class PaxAcceptor : public PaxPlayer{
        AcceptorState *acceptor_state_;
    public:
        void HandlePreProposeRequest(PaxMessage message);

        void HandleProposeRequest(PaxMessage message);
    };
}

#endif //PAXOSME_PAX_ACCEPTOR_HPP
