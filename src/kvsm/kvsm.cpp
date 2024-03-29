//
// Created by k s on 2021/12/19.
//

#include "common.hpp"
#include "kvsm.hpp"
#include "kv.pb.h"

#define SM_INSTANCE_ID "SM_INSTANCE_ID"

bool paxosme::KVStatemachine::Execute(instance_id_t instance_id, const std::string &paxos_value) {

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

    // update sm instance id
    SetSmInstanceId(instance_id);

    return true;
}

void paxosme::KVStatemachine::Init(SmOptions sm_options) {
    kv_->Init(sm_options.dbpath);
}

instance_id_t paxosme::KVStatemachine::GetSMInstanceId() {
    return 0;
}

bool paxosme::KVStatemachine::SetSmInstanceId(instance_id_t instance_id)
{
    //todo II: sm instance id 
    return true;
}

Statemachine* Statemachine:: New(){
    auto sm = new paxosme::KVStatemachine;
    return sm;
}



