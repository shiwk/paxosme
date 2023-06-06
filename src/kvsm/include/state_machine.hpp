//
// Created by shiwk on 2021/3/23.
//

#ifndef PAXOSME_STATE_MACHINE_HPP
#define PAXOSME_STATE_MACHINE_HPP

#include <sm.hpp>
#include <common.hpp>
#include <kv.hpp>

namespace paxosme {
    class KVStateMachine : public StateMachine {
    public:
        explicit KVStateMachine(SMReceipt);

        bool Execute(instance_id_t instance_id, const std::string &) override;

        void Init(SMOptions) override;

        instance_id_t GetSMInstanceId() override;

    private:
        std::unique_ptr<KV> kv_;
        SMReceipt smExecutionCtx_;
    };
}

#endif //PAXOSME_STATE_MACHINE_HPP
