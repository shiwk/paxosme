//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_CONTROLLER_HPP
#define PAXOSME_CONTROLLER_HPP

#include "messages_pax.hpp"
#include "proposer_pax.hpp"
#include "acceptor_pax.hpp"
#include "learner_pax.hpp"
#include "proposal_prov.hpp"
#include "sm.hpp"
#include <future>
#include <state_machine.hpp>

namespace paxosme {
    class PaxController {
    public:
        explicit PaxController(const PaxosOptions *);

        ~PaxController();

        void HandleMessage(const PaxMessage &message);

        void Init(Comm*, PaxStorage*); // start loop

        instance_id_t GetInstanceId() const;

        bool IsAccepted(instance_id_t i);

        const LogValue &GetAcceptedValue();

        ProposalTriplet GetAcceptedProposal();

        node_id_t GetAcceptedNodeId();

        void TryNewValue();

        void AddMessage(PaxMessage &);

        [[noreturn]] void *MainLoop(void*);

    private:
        void PushSMByState(instance_id_t target_instance_id);
        void NewInstance();
        void InstanceDone();

        static std::future<void *> prov_loop;

        Schedule schedule_;
        instance_id_t instance_id_;
        PaxProposer *proposer_;
        PaxAcceptor *acceptor_;
        PaxLearner *learner_;
        PaxosOptions *pax_config_;
        SafeQueue<PaxMessage*>* msgProv_;
        StateMachine *state_machine_;
        PaxCommunicator *communicator_;
    };
}

#endif //PAXOSME_CONTROLLER_HPP
