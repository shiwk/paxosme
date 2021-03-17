//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_DECIDER_PAX_HPP
#define PAXOSME_DECIDER_PAX_HPP

#include <paxos_node.hpp>
#include "player_pax.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class PaxDecider{
    public:
        bool IsMajorityAccepted();
        bool IsMajorityRejected();
        bool IsStillPending();
        bool AddApproval(proposal_id_t proposal_id, node_id_t node_id);
        bool AddRejection(proposal_id_t proposal_id, node_id_t node_id);
        void Reset();
        int GetMajorityCount();

    private:
        PaxConfig pax_config_;
        int approval_count_;
    };
}

#endif //PAXOSME_DECIDER_PAX_HPP
