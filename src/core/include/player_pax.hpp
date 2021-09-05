//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PLAYER_PAX_HPP
#define PAXOSME_PLAYER_PAX_HPP


#include <log_value.hpp>
#include <state.pb.h>
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
        virtual void Reset() = 0; // reset status for new instance
        PaxosState ReadState(instance_id_t instance_id);

        instance_id_t GetInstanceId() const;

    protected:
        PaxController *controller_;

        node_id_t GetNodeId() const;

        node_id_t GetFollowingNodeId() const;

        void ProcessChosenValue(const PaxMessage &message);

        bool IsAccepted(instance_id_t instance_id);

        void BroadCastMessage(const PaxMessage &message);

        void SendMessage(const PaxMessage &pax_message, node_id_t node_id);

        void WriteState(const paxosme::PaxosState &paxos_state);

        void AddTimer(EventType event_type, const event_callback &callback, int delayInMilli);

        ProposalTriplet GetAcceptedProposal();

        const LogValue &GetAcceptedValue();

    private:
        node_id_t node_id_;
        PaxCommunicate *communicate_;
        PaxStorage *storage_;
        Schedule *schedule_;
    };
}
#endif //PAXOSME_PLAYER_PAX_HPP
