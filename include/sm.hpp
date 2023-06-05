//
// Created by kk on 2022/12/4.
//

#ifndef PAXOSME_SM_HPP
#define PAXOSME_SM_HPP


using SMReceipt = void*;

//struct SMExecutionCtx {
//    void *receipt;
//};

class StateMachine {

public:
    struct SMOptions {
        std::string dbpath;
    };

    virtual void Init(SMOptions) = 0;

    virtual ~StateMachine() = default;

    // invoke after value learned
    // sm_ctx means context(user-defined struct) about the learned state
    virtual bool Execute(instance_id_t instance_id, const std::string &paxos_value) = 0;

    virtual instance_id_t GetSMInstanceId() = 0;

private:
};

#endif //PAXOSME_SM_HPP
