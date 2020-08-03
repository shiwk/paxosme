//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PLAYER_HPP
#define PAXOSME_PAX_PLAYER_HPP


#include "pax_messages.hpp"

namespace paxosme {
    class Communicator {
        void BroadCast(PaxMessage pax_message);
        void Send(PaxMessage pax_message, NodeId node_id);
    };

    class PaxPlayer {
    public:
        virtual void Send(PaxRequest pax_request, NodeId node_id) {}

        virtual void Send(PaxResponse pax_response, NodeId node_id) {}

        virtual void BroadCast(PaxMessage message) {}

        void GetInstanceId();

        void UpdateRoundId();

        void NewRound();

        void Reset(); // reset status for new instance

    private:
        Communicator communicator_;
    };
}
#endif //PAXOSME_PAX_PLAYER_HPP
