//
// Created by shiwk on 2021/3/5.
//

#include "storage_pax.hpp"
#include "state.pb.h"

//todo I:implement

int paxosme::PaxStorage::Write(const paxosme::PaxosStorageValue &value) {
    PaxosLogEntry paxos_log_entry;
    paxos_log_entry.set_accepted_node_id(value.acceptedNodeId);
    paxos_log_entry.set_accepted_proposal_id(value.acceptedProposalId);
    paxos_log_entry.set_accepted_value(value.acceptedValue);

    paxos_log_entry.set_instance_id(value.instanceId);

    paxos_log_entry.set_promised_node_id(value.promisedNodeId);
    paxos_log_entry.set_promised_proposal_id(value.promisedProposalId);

    paxos_log_entry.set_proposer_id(value.proposer);
    paxos_log_entry.set_leader_instance_id(value.leaderInstanceId);

    if (!logStorage_->Write(value.instanceId, paxos_log_entry.SerializeAsString()))
        return 0;

    return 1;
}

int paxosme::PaxStorage::Read(instance_id_t instance_id, paxosme::PaxosStorageValue &value) {
    LogEntry log_entry;
    if (logStorage_->Read(instance_id, log_entry)) {
        PaxosLogEntry paxos_log_entry;
        paxos_log_entry.ParseFromString(log_entry);
        value.instanceId = paxos_log_entry.instance_id();
        value.leaderInstanceId = paxos_log_entry.leader_instance_id();
        value.promisedProposalId = paxos_log_entry.promised_proposal_id();
        value.promisedNodeId = paxos_log_entry.promised_node_id();
        value.acceptedValue = paxos_log_entry.accepted_value();
        value.acceptedProposalId = paxos_log_entry.accepted_proposal_id();
        value.acceptedNodeId = paxos_log_entry.accepted_node_id();
        value.proposalId = paxos_log_entry.proposer_id();
        value.proposer = paxos_log_entry.proposer_id();

        return 1;
    }

    return 0;
}
