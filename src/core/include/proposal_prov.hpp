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
        void Submit(std::string &value);
        bool GetNewSubmit(instance_id_t instance_id, LogValue & log_value);

    private:
        instance_id_t instance_id_;
        LogValue* log_value_;
    };
}


#endif //PAXOSME_PROVIDER_PAX_HPP
