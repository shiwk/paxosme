//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_MESSAGES_HPP
#define PAXOSME_PAX_MESSAGES_HPP


#include <log_value.hpp>
#include "common.hpp"

namespace paxosme {
    class PaxMessage {
    public:
        PaxMessage(instance_id_t instance_id, proposal_id_t proposal_id, node_id_t node_id, const LogValue &log_value) : instance_id_(
                instance_id), proposal_id_(proposal_id), node_id_(node_id), log_value_(log_value) {}

    private:
        instance_id_t instance_id_;
        proposal_id_t proposal_id_;
        node_id_t node_id_;
        const LogValue &log_value_;

    public:
        proposal_id_t GetProposalId() const {
            return proposal_id_;
        }
    };


    class PaxPreReplyMessage{
    public:
        PaxPreReplyMessage(node_id_t proposer_id, node_id_t replier_id, proposal_id_t accepted_id, proposal_id_t promised_id, bool is_accepted) : accepted_id_(
                accepted_id), promised_id_(promised_id), is_accepted_(is_accepted) , proposer_id_(proposer_id), replier_id_(replier_id){}

    private:
        node_id_t proposer_id_;
    public:
        node_id_t GetProposerId() const {
            return proposer_id_;
        }

        node_id_t GetReplierId() const {
            return replier_id_;
        }

        proposal_id_t GetAcceptedId() const {
            return accepted_id_;
        }

        proposal_id_t GetPromisedId() const {
            return promised_id_;
        }

        bool IsAccepted() const {
            return is_accepted_;
        }

    private:
        node_id_t replier_id_;
        proposal_id_t accepted_id_;
        proposal_id_t promised_id_;
        bool is_accepted_;
    };

    struct Hoop {
    };
}

#endif //PAXOSME_PAX_MESSAGES_HPP
