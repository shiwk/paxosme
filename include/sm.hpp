//
// Created by kk on 2022/12/4.
//

#ifndef PAXOSME_SM_HPP
#define PAXOSME_SM_HPP
namespace paxosme {
    class StateMachine {
    public:
        void Init();

        virtual ~StateMachine() {}

        virtual bool Execute(instance_id_t instance_id, const std::string &sPaxosValue) = 0;

        instance_id_t GetInstanceId();
    };
}
#endif //PAXOSME_SM_HPP
