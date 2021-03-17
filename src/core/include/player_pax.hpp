//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PLAYER_PAX_HPP
#define PAXOSME_PLAYER_PAX_HPP


#include <log_value.hpp>
#include "messages_pax.hpp"
#include "pax_config.hpp"
#include "storage_pax.hpp"
#include "communicate_pax.hpp"

namespace paxosme {

    class PaxController;

    class PaxPlayer {
    public:
        virtual void Reset() = 0; // reset status for new instance

    protected:
        instance_id_t GetInstanceId() const;

        node_id_t GetNodeId() const;

        node_id_t GetFollowingNodeId() const;

        void ProcessChosenValue(const PaxMessage &message);

        bool IsAccepted(instance_id_t instance_id);

        void BroadCastMessage(const PaxMessage &message);

        void SendMessage(const PaxMessage &pax_message, node_id_t node_id);

        void Persist(const paxosme::PaxMessage &pax_message);
        void Persist(const LogValue &value, instance_id_t instance_id, proposal_id_t proposal_id, node_id_t proposer);

    private:
        node_id_t node_id_;
        PaxCommunicate *communicate_;
    protected:
        PaxController *controller_;
    };
}
#endif //PAXOSME_PLAYER_PAX_HPP
