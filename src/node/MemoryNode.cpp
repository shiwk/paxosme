//
// Created by k s on 2022/5/22.
//

#include "paxos_node.hpp"

namespace paxosme {
    MemoryNode::MemoryNode(paxosme::ConfigInfo *pax_config, Network *network, Storage *storage) : pax_config_(
            pax_config), network_(network), storage_(storage) {
        pax_controller_ = new PaxController(pax_config);
    }

    void MemoryNode::Launch() {
        pax_controller_->Init(reinterpret_cast<PaxCommunicator *>(network_->GetCommunicator()), nullptr);
    }
}