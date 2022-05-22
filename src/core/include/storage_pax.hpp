//
// Created by shiwk on 2021/3/5.
//

#ifndef PAXOSME_STORAGE_PAX_HPP
#define PAXOSME_STORAGE_PAX_HPP

#include "common.hpp"
#include <string>
#include <database.hpp>
#include <messages_pax.hpp>


namespace paxosme{
    struct PaxosStorageState{
        instance_id_t instanceId;
        proposal_id_t proposalId;
        node_id_t proposer;
        proposal_id_t acceptedProposalId;
        node_id_t acceptedNodeId;
        LogValue acceptedValue;
        node_id_t promisedNodeId;
        proposal_id_t promisedProposalId;
    };

    class PaxStorage {
    public:
        int Write(instance_id_t instance_id, const PaxosStorageState & value);

        int Read(instance_id_t instance_id, PaxosStorageState & value);
    private:
        Database * db;
    };
}


#endif //PAXOSME_STORAGE_PAX_HPP
