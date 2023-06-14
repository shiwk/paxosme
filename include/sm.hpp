//
// Created by kk on 2022/12/4.
//

#ifndef PAXOSME_SM_HPP
#define PAXOSME_SM_HPP


// struct SMExecutionCtx {
//     void *receipt;
// };

class Statemachine
{

public:
    using SmReceipt = void *;

    struct SmOptions
    {
        std::string dbpath;
        SmReceipt smReceipt;
    };

    virtual void Init(SmOptions) = 0;

    virtual ~Statemachine() = default;

    // invoke after value learned
    // sm_ctx means context(user-defined struct) about the learned state
    virtual bool Execute(instance_id_t instance_id, const std::string &paxos_value) = 0;

    virtual instance_id_t GetSMInstanceId() = 0;

    static Statemachine *New();

private:
};

#endif // PAXOSME_SM_HPP
