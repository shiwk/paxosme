//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_PAXOS_NODE_HPP
#define PAXOSME_PAXOS_NODE_HPP

#include <paxos_node.hpp>
#include <controller.hpp>

namespace paxosme {
    class PaxosNode : public Node{
    private:
        PaxController pax_controller_;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
