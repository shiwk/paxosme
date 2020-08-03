//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_PAX_INSTANCE_HPP
#define PAXOSME_PAX_INSTANCE_HPP

#include <common.hpp>
#include <log_value.hpp>

namespace paxosme {
    class PaxInstance {
        InstanceId GetInstanceId();
        LogValue GetInstanceValue();
        bool IsAlreadyChosen();

    private:
        InstanceId instance_id_;
        LogValue log_value_;
    };
}

#endif //PAXOSME_PAX_INSTANCE_HPP
