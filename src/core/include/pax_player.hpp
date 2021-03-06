//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PLAYER_HPP
#define PAXOSME_PAX_PLAYER_HPP


#include <log_value.hpp>
#include "messages_pax.hpp"
#include "pax_config.hpp"
#include "pax_storage.hpp"
#include "pax_communicate.hpp"

namespace paxosme {

    class PaxController;

    class PaxPlayer {
    public:
        virtual void Reset() = 0; // reset status for new instance

    protected:
        instance_id_t GetInstanceId() const;

        node_id_t GetNodeId() const;
        node_id_t GetFollowingNodeId() const;

        void ProcessAcceptedMessage(PaxMessage &pax_message);

        bool IsAccepted(instance_id_t instance_id);

        void BroadCastMessage(const PaxMessage &message, MessageType request_type);

        void SendMessage(const PaxMessage &pax_message, const node_id_t node_id, MessageType request_type);

        void Persist(const paxosme::PaxMessage &pax_message);

    private:
        node_id_t node_id_;
        PaxController *controller_;
        PaxStorage *storage_;
        PaxCommunicate * communicate_;
    };
}
#endif //PAXOSME_PAX_PLAYER_HPP
