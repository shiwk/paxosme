//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_MESSAGES_HPP
#define PAXOSME_PAX_MESSAGES_HPP


#include "common.hpp"

namespace paxosme {
    class PaxMessage {
    };

    class PaxRequest : PaxMessage{
    };

    class PaxResponse: PaxMessage{
    };

    struct Hoop{
    };
}

#endif //PAXOSME_PAX_MESSAGES_HPP
