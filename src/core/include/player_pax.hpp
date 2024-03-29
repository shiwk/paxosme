//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PLAYER_PAX_HPP
#define PAXOSME_PLAYER_PAX_HPP


#include <messages.hpp>
#include "config.hpp"
#include "storage_pax.hpp"
#include "communicate_pax.hpp"
#include <event.hpp>
#include <base.hpp>

namespace paxosme {

    class PaxController;

    struct ProposalTriplet{
        instance_id_t instance_id;
        proposal_id_t proposal_id;
        node_id_t node_id;
    };

    class PaxPlayer {
    public:
        explicit PaxPlayer(const PaxosOptions*, const PaxCommunicator*, const PaxStore *storage);
        virtual ~PaxPlayer() = default;
        PaxosLogEntry ReadState(instance_id_t instance_id);

        void InitController(const PaxController*);

        instance_id_t GetInstanceId() const;

        virtual void NewInstance() = 0;
        virtual void HandleMessage(const PaxMessage&) = 0;

    protected:
        PaxController *controller_;

        node_id_t GetNodeId() const;

        void ProcessChosenValue(const PaxMessage &message);

        bool IsAccepted(instance_id_t instance_id);

        void BroadCastMessage(const PaxMessage &message);

        void SendMessage(const PaxMessage &pax_message, node_id_t node_id);

        void WriteState(const paxosme::PaxosLogEntry &paxos_state);

        void Publish(EventType event_type, const EventHandler &callback, millisec = 0);
        void Withdraw(EventType);

        ProposalTriplet GetAcceptedProposal();

        const LogValue &GetAccepted();

    private:
        PaxCommunicator *communicator_;
        PaxStore *storage_;
        PaxosOptions *config_;
    };
}
#endif //PAXOSME_PLAYER_PAX_HPP
