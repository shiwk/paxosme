//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PROPOSER_HPP
#define PAXOSME_PAX_PROPOSER_HPP


#include <log_value.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"
#include "pax_decider.hpp"

namespace paxosme {

    enum class PaxProposerStatus {
        kNone = 0,
        kPrepare = 1,
        kAccept = 2
    };

    class PaxProposer : public PaxPlayer {

    public:
        void Propose(const LogValue& log_value);

        void HandlePreReply(PaxPreReplyMessage& pre_reply_message);

        void HandleAcceptReply(PaxMessage& message);

        void ProposeNoop();
        PaxProposer(const PaxConfig &pax_config);

    protected:
        void BroadCast(const PaxMessage &message);

    private:
        void PrePropose(const LogValue &log_value);

        void OnChosenValue(LogValue &log_value);

        void SetProposerStatus(paxosme::PaxProposerStatus proposer_status);

        PaxProposerStatus proposer_status_;
//        PaxMessage* proposed_message_;
        PaxDecider* pax_decider_;
    };
}
#endif //PAXOSME_PAX_PROPOSER_HPP
