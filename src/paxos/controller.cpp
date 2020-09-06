//
// Created by shiwk on 2020/8/1.
//

#include <controller.hpp>

instance_id_t paxosme::PaxController::GetInstanceId() const {
    return instance_id_;
}

void paxosme::PaxController::FinishInstance() {
    instance_id_ ++;
    // todo
}
