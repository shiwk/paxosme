//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PLAYER_HPP
#define PAXOSME_PAX_PLAYER_HPP


#include <log_value.hpp>
#include "pax_messages.hpp"
#include "pax_config.hpp"

namespace paxosme {

    enum RequestType{
        None,
        PreProposeBroadCast,
        ProposeBroadCast
    };

    class PaxRequest{
        PaxMessage pax_message_;
        RequestType request_type_;

    public:
        const PaxMessage &GetPaxMessage() const {
            return pax_message_;
        }

    public:
        PaxRequest(const PaxMessage &message, RequestType request_type) : pax_message_(message), request_type_(request_type){}
    };


    class Communicator {
    public:
        void BroadCast(const PaxRequest& pax_request);

        void Send(PaxMessage pax_message, node_id_t node_id);
    };

    class PaxState {
    public:
        void UpdateMaximalProposalId(proposal_id_t proposal_id) {
            max_proposal_id_ = max_proposal_id_ > proposal_id ? max_proposal_id_ : proposal_id;
        }

        proposal_id_t GetMaximalProposalId() {
            return max_proposal_id_;
        }

        proposal_id_t GetApplicableProposalId() {
            return max_proposal_id_ + 1;
        }

        instance_id_t GetInstanceId() const {
            return instance_id_;
        }

    private:
        proposal_id_t max_proposal_id_;
        instance_id_t instance_id_;
    };

    class PaxController;
    class PaxPlayer {
    public:
        virtual void Send(PaxRequest pax_request, node_id_t node_id) {}

        instance_id_t GetInstanceId();

        node_id_t GetNodeId() const {
            return node_id_;
        }
        void Reset(); // reset status for new instance

    protected:
        PaxMessage GeneratePreMessage() const;
        void ProcessMessageAcceptedByMajority(PaxMessage &pax_message);

        void BroadCastToAcceptors(const PaxRequest &pax_request);
        void BroadCastToLearners(const PaxMessage &message);

        Communicator *communicator_;
        PaxState *pax_state_;

    private:
        node_id_t node_id_;
        PaxController *controller_;
    };
}
#endif //PAXOSME_PAX_PLAYER_HPP
