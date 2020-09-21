//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PLAYER_HPP
#define PAXOSME_PAX_PLAYER_HPP


#include <log_value.hpp>
#include "pax_messages.hpp"
#include "pax_config.hpp"

namespace paxosme {

    enum RequestType {
        None,
        PreProposeBroadCast,
        ProposeBroadCast,
        PreProposeReply,
        ProposeReply
    };

    class Communicator {
    public:
        void BroadCast(const std::string &data);

        void BroadCast(const std::vector<byte> &data);
    };


    class PaxController;

    class PaxPlayer {
    public:
        virtual void Reset() = 0; // reset status for new instance

    protected:
        instance_id_t GetInstanceId() const;

        node_id_t GetNodeId() const;

        void ProcessAcceptedMessage(PaxMessage &pax_message);

        bool IsAccepted(instance_id_t instance_id);

        void BroadCast(const Serializable &message, RequestType request_type);

        void Send(const Serializable &message, const node_id_t node_id, RequestType request_type);

        void Persist(const instance_id_t instance_id, const LogValue &log_value);

    private:
        node_id_t node_id_;
        PaxController *controller_;
    };
}
#endif //PAXOSME_PAX_PLAYER_HPP
