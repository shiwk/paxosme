//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PROPOSER_PAX_HPP
#define PAXOSME_PROPOSER_PAX_HPP


#include <log_value.hpp>
#include "player_pax.hpp"
#include "pax_config.hpp"
#include "decider_pax.hpp"
#include "proposer_state.hpp"

namespace paxosme {

    enum class ProposerStatus : unsigned char {
        kNone = 0,
        kPrepare = 1,
        kPropose = 1 << 1,
        kMajorityAccepted = 1 << 2,
        kMajorityRejected = 1 << 3
    };

    inline constexpr int operator&(ProposerStatus a, unsigned char b) {
        return (static_cast<unsigned char>(a) & b);
    }

    class PaxProposer : public PaxPlayer {

    public:
        void ProposeNew();

        void HandlePrepareResponse(const PaxMessage &pax_reply_message);

        void HandleProposeResponse(const PaxMessage &pax_reply_message);

        PaxProposer(const PaxConfig &pax_config);

    private:
        PaxMessage GenerateMessage(MessageType message_type);

        void Prepare();

        void Propose();

        void HandleChosenValue(const PaxMessage &message);

        void HandleAbandonValue();

        ProposerStatus proposer_status_;
        PaxDecider *pax_decider_;
        ProposerState *proposer_state_;


        bool TryUpdateProposerStateWithAcceptorReply(const PaxMessage &message);
    };
}
#endif //PAXOSME_PROPOSER_PAX_HPP
