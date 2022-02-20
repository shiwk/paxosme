//
// Created by shiwk on 2020/8/2.
//

#ifndef PAXOSME_MSG_QUEUE_HPP
#define PAXOSME_MSG_QUEUE_HPP

namespace paxosme{
    class MsgQueue{
        void Push(PaxMessage pax_message);
        PaxMessage Pop(uint64_t);
        PaxMessage Peek();

        // todo I: thread safe message queue
    };
}


#endif //PAXOSME_MSG_QUEUE_HPP
