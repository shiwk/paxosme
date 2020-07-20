//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PLAYER_H
#define PAXOSME_PLAYER_H

#include "types/pax_message.h"

namespace paxosme{
    class PaxPlayer {
        void Send(PaxMessage message);
        void GetRoundId();
        void UpdateRoundId();
        void NewRound();
    private:
        RoundID round_id_;

    };
}


#endif //PAXOSME_PLAYER_H
