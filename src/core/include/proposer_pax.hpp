//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PROPOSER_PAX_HPP
#define PAXOSME_PROPOSER_PAX_HPP

#define PREPARE_TIMEOUT_CONST 50
#define PROPOSE_TIMEOUT_CONST 50

#include <log_value.hpp>
#include "player_pax.hpp"
#include "config_pax.hpp"
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
        void ProposeNew(LogValue &log_value);

        void HandlePrepareReply(const PaxMessage &pax_reply_message);

        void HandleProposeReply(const PaxMessage &pax_reply_message);

        PaxProposer(const PaxConfig &pax_config);

        void Init();

    private:
        PaxMessage GenerateMessage(MessageType message_type);

        void Prepare();
        void Prepare_Timeout_Callback(instance_id_t);

        void Propose();
        void Propose_Timeout_Callback(instance_id_t);

        void HandleChosenValue(const PaxMessage &message);

        void HandleAbandonValue();

        ProposerStatus proposer_status_;
        PaxDecider *pax_decider_;
        ProposerState *proposer_state_;


        bool TryUpdateProposerStateWithPrepareReply(const PaxMessage &message);

        static int PREPARE_DELAY;
        static int PROPOSE_DELAY;
    };
}
#endif //PAXOSME_PROPOSER_PAX_HPP
