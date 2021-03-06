//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PROPOSER_PAX_HPP
#define PAXOSME_PROPOSER_PAX_HPP


#include <log_value.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"
#include "pax_decider.hpp"
#include "proposer_state.hpp"

namespace paxosme {

    enum class ProposerStatus : unsigned char {
        kNone = 0,
        kPrePropose = 1,
        kPropose = 1 << 1,
        kMajorityAccepted = 1 << 2,
        kMajorityRejected = 1 << 3
    };

    inline constexpr int operator&(ProposerStatus a, unsigned char b) {
        return (static_cast<unsigned char>(a) & b);
    }

    class PaxProposer : public PaxPlayer {

    public:
        void ProposeNew(const LogValue &log_value);

        void HandlePreProposeResponse(const PaxAcceptorReplyMessage &pax_reply_message);

        void HandleProposeResponse(const PaxAcceptorReplyMessage &pax_reply_message);

        PaxProposer(const PaxConfig &pax_config);

    private:
        PaxMessage GeneratePreMessage(MessageType message_type);

        void PrePropose(const LogValue &log_value);

        void Propose();

        void HandleChosenValue();

        void HandleAbandonValue();

        ProposerStatus proposer_status_;
        PaxDecider *pax_decider_;
        ProposerState *proposer_state_;

        void LaunchPrePropose();

        void LaunchPropose();

        void SetPaxMessage(const PaxMessage &message);

        void UpdateLogValue(const LogValue &value);

        void HandleReceivedReply(const PaxAcceptorReplyMessage &pax_reply_message);

        bool TryUpdateProposerStateWithAcceptorReply(const PaxAcceptorReplyMessage &message);
    };
}
#endif //PAXOSME_PROPOSER_PAX_HPP
