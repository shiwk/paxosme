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
        ProposeBroadCast
    };

    class Communicator {
    public:
        void BroadCast(const std::string &data);
        void BroadCast(const std::vector<byte> &data);
    };


    class PaxController;

    class PaxPlayer {
    public:
        virtual void Reset()  = 0; // reset status for new instance

    protected:
        instance_id_t GetInstanceId() const;

        node_id_t GetNodeId() const ;
        void ProcessMessageAcceptedByMajority(PaxMessage &pax_message);

        void BroadCast(const PaxMessage &message, RequestType request_type);

    private:
        node_id_t node_id_;
        PaxController *controller_;
    };
}
#endif //PAXOSME_PAX_PLAYER_HPP
