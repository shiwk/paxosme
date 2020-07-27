//
// Created by shiwk on 2020/7/18.
//

#include "pax_proposer.hpp"

void paxosme::PaxProposer::SetProposerStatus(paxosme::PaxProposerStatus proposer_status) {
    proposer_status_ = proposer_status;
}

paxosme::PaxProposer::PaxProposer(const paxosme::PaxConfig &pax_config) : pax_config_(pax_config) {}
