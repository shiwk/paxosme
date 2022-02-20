//
// Created by shiwk on 2021/3/22.
//

#ifndef PAXOSME_PROVIDER_PAX_HPP
#define PAXOSME_PROVIDER_PAX_HPP

#include <log_value.hpp>
#include "common.hpp"

namespace paxosme {
    class ProposalProv {

    public:
        // todo I: cache for new proposing value and push the "NewValue" msg to message queue.
        // need waiting for the proposing value to be determined (success or fail)
        void Submit(std::string &value);

        bool GetNewSubmit(instance_id_t instance_id, LogValue &);

    private:
        instance_id_t instance_id_;

    };
}


#endif //PAXOSME_PROVIDER_PAX_HPP
