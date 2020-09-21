//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_CONTROLLER_HPP
#define PAXOSME_CONTROLLER_HPP


#include "pax_messages.hpp"
#include "msg_queue.hpp"
#include "pax_proposer.hpp"
#include "pax_acceptor.hpp"
#include "pax_learner.hpp"

namespace paxosme {
    class PaxController {
    public:
        void HandleReceivedMessage();

        void Propose(const PaxMessage &pax_message);

        void Pick(const PaxMessage &pax_message); // for proposer


        void Init(); // start loop

        instance_id_t GetInstanceId() const;

        void FinalizeInstance();

        bool IsAccepted(const instance_id_t i);

    private:
        MsgQueue msg_queue_;
        instance_id_t instance_id_;
        PaxProposer* proposer_;
        PaxAcceptor* acceptor_;
        PaxLearner* learner_;
    };
}

#endif //PAXOSME_CONTROLLER_HPP
