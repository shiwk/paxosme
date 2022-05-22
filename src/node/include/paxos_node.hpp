//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_PAXOS_NODE_HPP
#define PAXOSME_PAXOS_NODE_HPP

#include <controller.hpp>
#include <node.hpp>
#include <config.hpp>
#include "network.hpp"
#include "storage.hpp"

namespace paxosme {
    class MemoryNode : public Node {
    public:
        MemoryNode(paxosme::ConfigInfo *, Network *, Storage *);

        void Launch() override;

    private:
        ConfigInfo *pax_config_;
        PaxController *pax_controller_;
        PaxCommunicator *pax_communicator_;
        Network *network_;
        Storage *storage_;
    };

    class PersistNode : public Node {
        // todo II: Persisting node
    };

}
#endif //PAXOSME_PAXOS_NODE_HPP
