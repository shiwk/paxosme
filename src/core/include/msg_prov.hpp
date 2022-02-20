//
// Created by shiwk on 2020/8/2.
//

#ifndef PAXOSME_MSG_PROV_HPP
#define PAXOSME_MSG_PROV_HPP

#include <queue>
#include "lock.hpp"
#include "messages_pax.hpp"

namespace paxosme{
    class MsgProv{
    public:
        // todo I: new value and network message invoking
        void Add(PaxMessage* pax_message);

        // get and pop
        bool Take(PaxMessage *, std::chrono::milliseconds waitTime = std::chrono::milliseconds(1000));

        // pop
        void PopN(size_t);

    private:
        std::queue<PaxMessage*> queue_;
        MyLock lock_;
    };
}


#endif //PAXOSME_MSG_PROV_HPP
