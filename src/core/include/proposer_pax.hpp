//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PROPOSER_PAX_HPP
#define PAXOSME_PROPOSER_PAX_HPP

#define PROPOSE_TIMEOUT_CONST 50
#define ACCEPT_TIMEOUT_CONST 50

#include "player_pax.hpp"
#include "config.hpp"
#include "proposal_prov.hpp"

namespace paxosme {

    enum class ProposerStatus : unsigned char {
        kNone = 0,
        kPropose = 1,
        kAccept = 1 << 1,
        kMajorityAccepted = 1 << 2,
        kMajorityRejected = 1 << 3
    };

    inline constexpr int operator&(ProposerStatus a, unsigned char b) {
        return (static_cast<unsigned char>(a) & b);
    }

    class ProposerState {

        LogValue log_value_;
        proposal_id_t highest_known_proposal_;
        proposal_id_t my_proposal_id_;
        node_id_t highest_proposal_provider_;

    public:
        ProposerState();

        proposal_id_t GetMyProposalId() const;

        void Init(proposal_id_t);

        const LogValue &GetLogValue() const;

        void SetLogValue(const LogValue &);

        bool TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t);

        bool TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id, const LogValue &log_value);

        proposal_id_t NewProposalId();

        void Reset();
    };

    class ProposalCounter {
    public:
        explicit ProposalCounter(const PaxosOptions *);

        bool IsMajorityAccepted();

        bool IsMajorityPromised();

        bool IsMajorityRejected();

        bool IsStillPending();

        bool AddAccept(proposal_id_t, node_id_t);

        bool AddPromise(proposal_id_t, node_id_t);

        bool AddRejection(proposal_id_t, node_id_t);

        void Reset();

        bool SomeoneReject();

    private:
        PaxosOptions *pax_config_;
        int32_t vote_count_;

        std::set<node_id_t> approvals_;
        std::set<node_id_t> promises_;
        std::set<node_id_t> rejections_;
    };

    class PaxProposer : public PaxPlayer {

    public:
        explicit PaxProposer(const PaxosOptions *, const PaxCommunicator *, const PaxStore *);

        void NewValue();

        void Init(proposal_id_t, const PaxController *);

        void InstanceDone(instance_id_t instance_id, const LogValue &log_value);

        void NewInstance() override;

        void HandleMessage(const PaxMessage &) override;


    private:
        PaxMessage GenerateMessage(MessageType, proposal_id_t);

        void HandleProposeAck(const PaxMessage &pax_reply_message);

        void HandleAcceptAck(const PaxMessage &message);

        void Propose();

        void Accept();

        void ProposeTimeoutCallback(instance_id_t instanceId);

        void NewValueTimeoutCallback();

        ProposerStatus status_;
        ProposalCounter proposal_counter_;
        ProposerState proposer_state_;
        ProposalProv *proposal_prov_{};


        bool TryUpdateProposerStateWithPrepareReply(const PaxMessage &message);

        static int PROPOSE_TIMEOUT;
        static int ACCEPT_TIMEOUT;
    };
}
#endif //PAXOSME_PROPOSER_PAX_HPP
