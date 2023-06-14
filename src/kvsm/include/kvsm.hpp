//
// Created by shiwk on 2021/3/23.
//

#ifndef PAXOSME_STATE_MACHINE_HPP
#define PAXOSME_STATE_MACHINE_HPP

#include <sm.hpp>
#include <common.hpp>
#include <kv.hpp>

namespace paxosme {
    class KVStatemachine : public Statemachine {
        friend class Statemachine;
    public:

        bool Execute(instance_id_t instance_id, const std::string &) override;

        void Init(SmOptions) override;

        instance_id_t GetSMInstanceId() override;

        bool SetSmInstanceId(instance_id_t instance_id);

    private:
        std::unique_ptr<KV> kv_;
        SmReceipt smExecutionCtx_;
    };
}

#endif //PAXOSME_STATE_MACHINE_HPP
