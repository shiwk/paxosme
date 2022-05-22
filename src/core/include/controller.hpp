//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_CONTROLLER_HPP
#define PAXOSME_CONTROLLER_HPP

#include "messages_pax.hpp"
#include "queue.hpp"
#include "proposer_pax.hpp"
#include "acceptor_pax.hpp"
#include "learner_pax.hpp"
#include "proposal_prov.hpp"
#include <future>
#include <state_machine.hpp>

namespace paxosme {
    class PaxController {
    public:
        explicit PaxController(const ConfigInfo *);

        ~PaxController();

        void HandleMessage(const PaxMessage &message);

        void Init(PaxCommunicator*, PaxStorage*); // start loop

        instance_id_t GetInstanceId() const;

        bool IsAccepted(instance_id_t i);

        const LogValue &GetAcceptedValue();

        ProposalTriplet GetAcceptedProposal();

        node_id_t GetAcceptedNodeId();

        void TryNewValue();

        void AddMessage(PaxMessage &);

        [[noreturn]] void *MainLoop();

    private:
        void PushSMByState(instance_id_t target_instance_id);
        void NewInstance();
        void InstanceDone();

        Schedule schedule_;
        instance_id_t instance_id_;
        PaxProposer *proposer_;
        PaxAcceptor *acceptor_;
        PaxLearner *learner_;
        ConfigInfo *pax_config_;
        std::future<void *> prov_loop_;
        MyQueue<PaxMessage*> msgProv_;
        StateMachine *state_machine_;
    };
}

#endif //PAXOSME_CONTROLLER_HPP
