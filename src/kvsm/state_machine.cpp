//
// Created by k s on 2021/12/19.
//

#include "common.hpp"
#include "state_machine.hpp"

bool KVStateMachine::Execute(instance_id_t instance_id, const std::string &paxos_value) {
    // todo I:
    //  execute paxos_value(GET AND PUT)
    //  fill sm receipt
    //  set SMInstanceId


    return false;
}

void KVStateMachine::Init(SMOptions sm_options) {
    kv_->Init(sm_options.dbpath);
}

instance_id_t KVStateMachine::GetSMInstanceId() {
    return 0;
}

KVStateMachine::KVStateMachine(SMReceipt sm_receipt) : smExecutionCtx_(sm_receipt) {
    kv_ = std::make_unique<KV>();
}



