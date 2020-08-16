//
// Created by shiwk on 2020/8/1.
//

#ifndef PAXOSME_CONTROLLER_HPP
#define PAXOSME_CONTROLLER_HPP


#include "pax_messages.hpp"
#include "msg_queue.hpp"
#include "pax_proposer.hpp"

namespace paxosme {
    class PaxController {
    public:
        void Propose(const PaxMessage &pax_message);

        void AddMessage(const PaxMessage &pax_message);

        void AddPendingMessage(const PaxMessage &pax_message);

        void Init(); // start loop

    private:
        MsgQueue msg_queue_;
        PaxProposer pax_proposer_;
    };
}

#endif //PAXOSME_CONTROLLER_HPP