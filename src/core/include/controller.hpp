//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_CONTROLLER_HPP
#define PAXOSME_CONTROLLER_HPP

#include "messages_pax.hpp"
#include "msg_queue.hpp"
#include "proposer_pax.hpp"
#include "acceptor_pax.hpp"
#include "learner_pax.hpp"
#include "proposal_prov.hpp"
#include <future>
#include <state_machine.hpp>

namespace paxosme {
    class PaxController {
    public:
        explicit PaxController(const PaxConfig *, const PaxCommunicator *, const Storage *, const Schedule *);
        void HandleMessage(const PaxMessage &message);

        void Init(); // start loop

        instance_id_t GetInstanceId() const;

        bool IsAccepted(const instance_id_t i);

        const LogValue &GetAcceptedValue();

        ProposalTriplet GetAcceptedProposal();

        node_id_t GetAcceptedNodeId();

        [[noreturn]] void FlushProv();

        void NewInstance();

    private:
        void PushSMByState(instance_id_t target_instance_id);

    private:
        MsgQueue msg_queue_;
        instance_id_t instance_id_;
        PaxProposer proposer_;
        PaxAcceptor acceptor_;
        PaxLearner learner_;
        PaxConfig *pax_config_;
        ProposalProv *proposal_prov_;
        std::future <void*> prov_loop_;
        StateMachine *state_machine_;
    };
}

#endif //PAXOSME_CONTROLLER_HPP
