//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PLAYER_PAX_HPP
#define PAXOSME_PLAYER_PAX_HPP


#include <log_value.hpp>
#include "messages_pax.hpp"
#include "config_pax.hpp"
#include "storage_pax.hpp"
#include "communicate_pax.hpp"
#include "schedule.hpp"
#include <base.hpp>

namespace paxosme {

    class PaxController;

    class PaxPlayer {
    public:
        explicit PaxPlayer(const PaxConfig*, const PaxCommunicator*, const Storage *storage, const Schedule *schedule);
        PaxosState ReadState(instance_id_t instance_id);

        void InitController(const PaxController*);

        instance_id_t GetInstanceId() const;

        virtual void NewInstance() = 0;

    protected:
        PaxController *controller_;

        node_id_t GetNodeId() const;

        void ProcessChosenValue(const PaxMessage &message);

        bool IsAccepted(instance_id_t instance_id);

        void BroadCastMessage(const PaxMessage &message);

        void SendMessage(const PaxMessage &pax_message, node_id_t node_id);

        void WriteState(const paxosme::PaxosState &paxos_state);

        void Publish(EventType event_type, const event_callback &callback, int delayInMilli = 0);

        ProposalTriplet GetAcceptedProposal();

        const LogValue &GetAccepted();

    private:
        PaxCommunicator *communicator_;
        Storage *storage_;
        Schedule *schedule_;
        PaxConfig *config_;
    };
}
#endif //PAXOSME_PLAYER_PAX_HPP
