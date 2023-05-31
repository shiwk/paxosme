//
// Created by kk on 2022/12/4.
//

#ifndef PAXOSME_SM_HPP
#define PAXOSME_SM_HPP

struct SMCtx {
    int smId;
    void *context;
};

class StateMachine {

public:
    struct SMOptions {
    };

    virtual void Init() = 0;

    virtual ~StateMachine() = default;

    // invoke after value learned
    // sm_ctx means context(user-defined struct) about the learned state
    virtual bool Execute(instance_id_t instance_id, const std::string &sPaxosValue, SMCtx *sm_ctx) = 0;

    virtual instance_id_t GetInstanceId() = 0;

private:
};

#endif //PAXOSME_SM_HPP
