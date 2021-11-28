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

        void Init(proposal_id_t);

        void NewInstance() override;
    private:
        PaxMessage GenerateMessage(MessageType, proposal_id_t);

        void Prepare(bool newPrepare);
        void Propose();
        void ProposerTimeoutCallback(instance_id_t instanceId, bool needNewPrepare);

        ProposerStatus proposer_status_;
        PaxDecider *proposal_decider_;
        ProposerState *proposer_state_;


        bool TryUpdateProposerStateWithPrepareReply(const PaxMessage &message);

        static int PREPARE_TIMEOUT;
        static int PROPOSE_TIMEOUT;
    };
}
#endif //PAXOSME_PROPOSER_PAX_HPP
