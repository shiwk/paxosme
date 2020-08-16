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
        kPrePropose = 1,
        kPropose = 2
    };

    class PaxProposer : public PaxPlayer {

    public:
        void ProposeNew(const LogValue& log_value);

        void HandlePreReply(const PaxReplyMessage& pax_reply_message);

        void HandleProposeReply(const PaxReplyMessage& pax_reply_message);

        void ProposeNoop();

        PaxProposer(const PaxConfig &pax_config);

    private:
        void PrePropose(const LogValue &log_value);
        void Propose();

        void OnChosenValue();
        PaxProposerStatus proposer_status_;
        PaxMessage proposed_message_;
        PaxDecider* pax_decider_;

        void LaunchPrePropose();
        void LaunchPropose();
        void SetPaxMessage(const PaxMessage &message);

        void SetLogValue(const LogValue &value);

        void OnReceivedReply(const PaxReplyMessage &pax_reply_message);

        void UpdatePaxMessage();
    };
}
#endif //PAXOSME_PAX_PROPOSER_HPP
