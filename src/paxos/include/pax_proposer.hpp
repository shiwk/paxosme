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

    enum class ProposerStatus {
        kNone = 0,
        kPrePropose = 1,
        kPropose = 2
    };

    class PaxProposer : public PaxPlayer {

    public:
        void ProposeNew(const LogValue &log_value);

        void HandlePreProposeResponse(const PaxReplyMessage &pax_reply_message);

        void HandleProposeResponse(const PaxReplyMessage &pax_reply_message);

        void ProposeNoop();

        PaxProposer(const PaxConfig &pax_config);

    private:
        void PrePropose(const LogValue &log_value);

        void Propose();

        void OnChosenValue();

        ProposerStatus proposer_status_;
        PaxDecider *pax_decider_;
        ProposerState *proposer_state_;

        void LaunchPrePropose();

        void LaunchPropose();

        void SetPaxMessage(const PaxMessage &message);

        void UpdateLogValue(const LogValue &value);

        void OnReceivedReply(const PaxReplyMessage &pax_reply_message);

        void TryUpdatePaxMessageWithPreProposeReply(proposal_id_t proposal_id, const LogValue value);

        void TryUpdatePaxMessageWithPreProposeReply(const PaxReplyMessage message);
    };
}
#endif //PAXOSME_PAX_PROPOSER_HPP
