//
// Created by shiwk on 2021/3/5.
//

#include "storage_pax.hpp"

//todo I:implement

int paxosme::PaxStore::Write(const paxosme::PaxosLogEntry &value) {
    // PaxosLogEntry paxos_log_entry;
    // paxos_log_entry.set_accepted_node_id(value.acceptedNodeId());
    // paxos_log_entry.set_accepted_proposal_id(value.acceptedProposalId);
    // paxos_log_entry.set_accepted_value(value.acceptedValue);

    // paxos_log_entry.set_instance_id(value.instanceId);

    // paxos_log_entry.set_promised_node_id(value.promisedNodeId);
    // paxos_log_entry.set_promised_proposal_id(value.promisedProposalId);

    // paxos_log_entry.set_proposer_id(value.proposer);
    // paxos_log_entry.set_leader_instance_id(value.leaderInstanceId);

    if (!logStorage_->Put(std::to_string(value.instance_id()), value.SerializeAsString()))
        return 0;

    return 1;
}

int paxosme::PaxStore::Read(instance_id_t instance_id, paxosme::PaxosLogEntry &value) {
    LogStorage::LogEntryValue log_entry_value;
    if (logStorage_->Get(std::to_string(instance_id), log_entry_value)) {
        value.ParseFromString(log_entry_value);

        return 1;
    }

    return 0;
}
