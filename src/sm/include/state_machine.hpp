//
// Created by shiwk on 2021/3/23.
//

#ifndef PAXOSME_STATE_MACHINE_HPP
#define PAXOSME_STATE_MACHINE_HPP


//todo II: sm
class StateMachine {
public:

    virtual ~StateMachine() {}

    virtual bool Execute(instance_id_t instance_id, const std::string &sPaxosValue) = 0;

    instance_id_t GetInstanceId();
};

#endif //PAXOSME_STATE_MACHINE_HPP
