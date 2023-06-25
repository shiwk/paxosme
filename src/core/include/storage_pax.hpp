//
// Created by shiwk on 2021/3/5.
//

#ifndef PAXOSME_STORAGE_PAX_HPP
#define PAXOSME_STORAGE_PAX_HPP

#include "common.hpp"
#include <string>
#include <messages_pax.hpp>
#include "logstorage.hpp"
#include <state.pb.h>



namespace paxosme{
    // struct PaxosLogEntry{
    //     instance_id_t instanceId;
    //     proposal_id_t proposalId;
    //     node_id_t proposer;
    //     proposal_id_t acceptedProposalId;
    //     node_id_t acceptedNodeId;
    //     LogValue acceptedValue;
    //     node_id_t promisedNodeId;
    //     proposal_id_t promisedProposalId;
    //     proposal_id_t leaderInstanceId;
    // };

    class PaxStore {
    public:
        int WriteLog(const PaxosLogEntry & value);

        int ReadLog(instance_id_t instance_id, PaxosLogEntry & value);
    private:
        LogStorage *logStorage_;
    };
}


#endif //PAXOSME_STORAGE_PAX_HPP
