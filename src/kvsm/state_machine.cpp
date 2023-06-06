//
// Created by k s on 2021/12/19.
//

#include "common.hpp"
#include "state_machine.hpp"
#include "kv.pb.h"

bool paxosme::KVStateMachine::Execute(instance_id_t instance_id, const std::string &paxos_value) {

    paxosme::PaxosKVPB paxos_kvpb;
    std::string value;
    version_id ver;
    paxos_kvpb.ParseFromArray(paxos_value.data(), paxos_value.size());

    bool execution = false;
    if (paxos_kvpb.method() == paxosme::PaxosKVPB_KVMethod::PaxosKVPB_KVMethod_GET) {
        execution = kv_->Get(paxos_kvpb.key(), value, ver);
    } else if (paxos_kvpb.method() == paxosme::PaxosKVPB_KVMethod::PaxosKVPB_KVMethod_PUT) {
        execution = kv_->Put(paxos_kvpb.key(), paxos_kvpb.value(), paxos_kvpb.ver());
    }

    if (!execution){
        // sm execution failed
        return false;
    }

    // todo I:
    //  update SM InstanceId

    return true;
}

void paxosme::KVStateMachine::Init(SMOptions sm_options) {
    kv_->Init(sm_options.dbpath);
}

instance_id_t paxosme::KVStateMachine::GetSMInstanceId() {
    return 0;
}

paxosme::KVStateMachine::KVStateMachine(SMReceipt sm_receipt) : smExecutionCtx_(sm_receipt) {
    kv_ = std::make_unique<KV>();
}



