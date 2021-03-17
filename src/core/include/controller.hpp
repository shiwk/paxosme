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

namespace paxosme {
    class PaxController {
    public:
        void HandleReceivedMessage(const PaxMessage &message);

        void Propose(const PaxMessage &pax_message);

        void Pick(const PaxMessage *pax_message); // for proposer


        void Init(); // start loop

        instance_id_t GetInstanceId() const;

        void FinalizeInstance();

        bool IsAccepted(const instance_id_t i);

        const LogValue &GetAcceptedValue();

        proposal_id_t GetAcceptedProposal();

        node_id_t GetAcceptedNodeId();

    private:
        MsgQueue msg_queue_;
        instance_id_t instance_id_;
        PaxProposer *proposer_;
        PaxAcceptor *acceptor_;
        PaxLearner *learner_;
        PaxStorage *storage_;
        bool is_init_;
    };
}

#endif //PAXOSME_CONTROLLER_HPP
