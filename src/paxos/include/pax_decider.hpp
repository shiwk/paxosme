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
        bool IsToBeReceived();

    private:
        int GetMajorityCount();
        PaxConfig pax_config_;
        NodeConfig node_config_;
    };
}

#endif //PAXOSME_PAX_DECIDER_HPP
