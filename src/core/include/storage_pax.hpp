//
// Created by shiwk on 2021/3/5.
//

#ifndef PAXOSME_STORAGE_PAX_HPP
#define PAXOSME_STORAGE_PAX_HPP

#include "common.hpp"
#include <string>
#include <database.hpp>
#include <messages_pax.hpp>
#include "logstorage.hpp"


namespace paxosme{
    struct PaxosStorageValue{
        instance_id_t instanceId;
        proposal_id_t proposalId;
        node_id_t proposer;
        proposal_id_t acceptedProposalId;
        node_id_t acceptedNodeId;
        LogValue acceptedValue;
        node_id_t promisedNodeId;
        proposal_id_t promisedProposalId;
        proposal_id_t leaderInstanceId;
    };

    class PaxStorage {
    public:
        int Write(const PaxosStorageValue & value);

        int Read(instance_id_t instance_id, PaxosStorageValue & value);
    private:
        LogStorage *logStorage_;
    };
}


#endif //PAXOSME_STORAGE_PAX_HPP
