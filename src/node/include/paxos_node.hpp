//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_PAXOS_NODE_HPP
#define PAXOSME_PAXOS_NODE_HPP

#include <controller.hpp>
#include <node.hpp>

namespace paxosme {
    class PaxosNode : public Node{
    private:
        PaxController pax_controller_;
    };
}

#endif //PAXOSME_PAXOS_NODE_HPP
