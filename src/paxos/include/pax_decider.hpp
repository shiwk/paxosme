//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_DECIDER_HPP
#define PAXOSME_PAX_DECIDER_HPP

#include <paxos_node.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class PaxDecider{
    public:
        bool IsToBeAccepted();
        void AddApproval(proposal_id_t proposal_id, node_id_t node_id);
        void Reset();

    private:
        int GetMajorityCount();
        PaxConfig pax_config_;
        int approval_count_;

    };
}

#endif //PAXOSME_PAX_DECIDER_HPP
