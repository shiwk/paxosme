//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PROPOSER_HPP
#define PAXOSME_PAX_PROPOSER_HPP


#include <log_value.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"
#include "pax_decider.hpp"
#include "proposer_state.hpp"

namespace paxosme {

    enum class ProposerStatus : unsigned char {
        kNone = 0,
        kPrePropose = 1,
        kPropose = 1u << 1u,
        kMajorityAccepted = 1u << 2u,
        kMajorityRejected = 1u << 3u
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
        PaxMessage GeneratePreMessage() const;

        void PrePropose(const LogValue &log_value);

        void Propose();

        void OnChosenValue();

        void OnAbandonValue();

        ProposerStatus proposer_status_;
        PaxDecider *pax_decider_;
        ProposerState *proposer_state_;

        void LaunchPrePropose();

        void LaunchPropose();

        void SetPaxMessage(const PaxMessage &message);

        void UpdateLogValue(const LogValue &value);

        void OnReceivedReply(const PaxAcceptorReplyMessage &pax_reply_message);

        bool TryUpdateProposerStateWithRejectionReply(const PaxAcceptorReplyMessage &message);
    };
}
#endif //PAXOSME_PAX_PROPOSER_HPP
