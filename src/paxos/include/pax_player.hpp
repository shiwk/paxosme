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
        PaxMessage* pax_message_;
        RequestType request_type_;

    public:
        PaxRequest(PaxMessage &message) : pax_message_(&message){}
    };

    class PaxResponse : PaxMessage {
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

        virtual void Send(PaxResponse pax_response, node_id_t node_id) {}


        instance_id_t GetInstanceId();


        void Reset(); // reset status for new instance

    protected:
        PaxMessage GenerateMessage(const LogValue &log_value) const;
        virtual void ProcessMessageAcceptedByMajority(PaxMessage &pax_message);
        virtual void BroadCast(const PaxMessage &message) ;

        Communicator *communicator_;
        PaxState *pax_state_;

    private:
        node_id_t node_id_;
        PaxController *controller_;

    public:
        node_id_t GetNodeId() const {
            return node_id_;
        }
    };
}
#endif //PAXOSME_PAX_PLAYER_HPP
