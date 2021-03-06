//
// Created by shiwk on 2020/8/16.
//

#ifndef PAXOSME_PROPOSER_STATE_HPP
#define PAXOSME_PROPOSER_STATE_HPP


#include "messages_pax.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class ProposerState {
        PaxMessage pending_message_;

    private:
        proposal_id_t highest_proposal_id_;
        node_id_t highest_proposal_id_provider_;
        instance_id_t instance_id_;
        PaxConfig *pax_config_;

    public:
        ProposerState(const PaxMessage &pending_message);

        bool TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t node_id);

        bool
        TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id, const LogValue &log_value);

        proposal_id_t GetApplicableProposalId() {
            return highest_proposal_id_ + 1;
        }

        instance_id_t GetInstanceId() const {
            return instance_id_;
        }

        proposal_id_t GetPendingProposalId() {
            return pending_message_.GetProposalId();
        }
        const PaxMessage &GetPendingMessage() const {
            return pending_message_;
        }
        void SetPendingMessage(const PaxMessage &message) {
            pending_message_ = message;
        }
        void SetLogValue(const LogValue &value) {
            pending_message_.SetLogValue(value);
        }
    };
}


#endif //PAXOSME_PROPOSER_STATE_HPP
