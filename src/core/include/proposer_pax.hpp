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

    class ProposerState {
        // todo I: initialization
        LogValue& log_value_;
        proposal_id_t highest_known_proposal_;
        proposal_id_t my_proposal_Id_;
        node_id_t highest_proposal_id_provider_;

    public:

        proposal_id_t GetMyProposalId() const;
        void Init(proposal_id_t);
        const LogValue &GetLogValue() const;

        void SetLogValue(LogValue &log_value);

        bool TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t);

        bool TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id, const LogValue &log_value);

        proposal_id_t NewProposalId();
        void Reset();
    };

    class ProposalCounter{
    public:
        bool IsMajorityAccepted();
        bool IsMajorityPromised();
        bool IsMajorityRejected();
        bool IsStillPending();
        bool AddAccept(proposal_id_t, node_id_t);
        bool AddPromise(proposal_id_t, node_id_t);
        bool AddRejection(proposal_id_t, node_id_t);
        void Reset();

    private:
        PaxConfig* pax_config_;
        int32_t vote_count_;

        std::set<node_id_t> approvals_;
        std::set<node_id_t> promises_;
        std::set<node_id_t> rejections_;
    };

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
        ProposalCounter *proposal_decider_;
        ProposerState proposer_state_;


        bool TryUpdateProposerStateWithPrepareReply(const PaxMessage &message);

        static int PREPARE_TIMEOUT;
        static int PROPOSE_TIMEOUT;
    };
}
#endif //PAXOSME_PROPOSER_PAX_HPP
