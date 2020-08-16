//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_MESSAGES_HPP
#define PAXOSME_PAX_MESSAGES_HPP


#include <log_value.hpp>
#include <utility>
#include <string>
#include "common.hpp"

namespace paxosme {
    class PaxPreMessage{

    };

    class PaxMessage {
    public:
        PaxMessage(instance_id_t instance_id, proposal_id_t proposal_id, node_id_t node_id)
                : instance_id_(instance_id), proposal_id_(proposal_id), node_id_(node_id){}
    private:
        instance_id_t instance_id_;
        proposal_id_t proposal_id_;
        node_id_t node_id_;
        LogValue log_value_;

    public:
        void SetLogValue(const LogValue &log_value) {
            log_value_ = log_value;
        }

        const LogValue &GetLogValue() const {
            return log_value_;
        }

        proposal_id_t GetProposalId() const {
            return proposal_id_;
        }
        void SetProposalId(proposal_id_t proposal_id) {
            proposal_id_ = proposal_id;
        }
    };


    class PaxReplyMessage {
    public:
        PaxReplyMessage(node_id_t proposer_id, node_id_t replier_id, proposal_id_t accepted_id,
                        proposal_id_t promised_id, bool is_accepted) : accepted_id_(
                accepted_id), promised_id_(promised_id), is_accepted_(is_accepted), proposer_id_(proposer_id),
                                                                          replier_id_(replier_id) {}

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
