//
// Created by shiwk on 2020/8/1.
//

#include <controller.hpp>

instance_id_t paxosme::PaxController::GetInstanceId() const {
    return instance_id_;
}

void paxosme::PaxController::FinalizeInstance() {
    instance_id_ ++;
    // todo
}

bool paxosme::PaxController::IsAccepted(const instance_id_t i) {
    return acceptor_->IsAccepted();
}

const paxosme::LogValue &paxosme::PaxController::GetAcceptedValue() {
    return acceptor_->GetAcceptedValue();
}

node_id_t paxosme::PaxController::GetAcceptedNodeId() {
    return acceptor_->GetAcceptedNodeId();
}

void paxosme::PaxController::Init() {
    instance_id_ = storage_->ReadLastInstance();
    // todo II : if instance_id > instance id in checkpoint, it means needs to execute some log_values
    // todo III : if instance_id < instance id in checkpoint, it means some special cases (like restart/crash, even worse) happened
    acceptor_->init();
    is_init_ = true;
}
