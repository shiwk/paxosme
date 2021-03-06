//
// Created by shiwk on 2020/8/2.
//

#ifndef PAXOSME_MSG_QUEUE_HPP
#define PAXOSME_MSG_QUEUE_HPP

namespace paxosme{
    class MsgQueue{
        void Push(PaxMessage pax_message);
        PaxMessage Pop();
        PaxMessage Peek();
    };
}

#endif //PAXOSME_MSG_QUEUE_HPP
