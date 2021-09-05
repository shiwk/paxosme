//
// Created by k s on 2021/9/5.
//
#include <learner_pax.hpp>

namespace paxosme {
    bool PaxLearner::Learned() {
        return false;
    }

    bool PaxLearner::HandleSenderPublish(const PaxMessage &pax_message) {
        LearnFromSelf(pax_message);
        return true;
    }

    void PaxLearner::HandleLeaderPublish(const PaxMessage& pax_message) {
        LearnFromOthers(pax_message);
    }

    void PaxLearner::LearnNew(const LogValue &value, instance_id_t instance_id, proposal_id_t proposal_id,
                              node_id_t proposer, bool writeState) {
        learner_state_->LearnNew(value, instance_id, proposal_id, proposer);
        if (writeState) {
            PaxosState paxos_state;
            paxos_state.set_promised_proposal_id(proposal_id);
            paxos_state.set_promised_node_id(proposer);
            paxos_state.set_instance_id(instance_id);
            paxos_state.set_accepted_proposal_id(proposal_id);
            paxos_state.set_accepted_node_id(proposer);
            paxos_state.set_accepted_value(value.GetValue());
            WriteState(paxos_state);
        }
    }
}