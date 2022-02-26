//
// Created by shiwk on 2021/3/22.
//

#ifndef PAXOSME_PROVIDER_PAX_HPP
#define PAXOSME_PROVIDER_PAX_HPP

#include <log_value.hpp>
#include "common.hpp"
#include "lock.hpp"

namespace paxosme {
    class ProposalProv {

    public:
        // todo I: cache for new proposing value and push the "NewValue" msg to message queue.
        // need waiting for the proposing value to be determined (success or fail)
        void NewValue(std::string &value);

        bool Get(instance_id_t instance_id, LogValue &);

    private:
        LogValue logValue;
        MyLock lock_;
    };
}


#endif //PAXOSME_PROVIDER_PAX_HPP
