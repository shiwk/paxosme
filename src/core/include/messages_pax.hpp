//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_MESSAGES_PAX_HPP
#define PAXOSME_MESSAGES_PAX_HPP


#include <log_value.hpp>
#include <utility>
#include <string>
#include "common.hpp"

namespace paxosme {

    enum MessageType {
        None,
        PreProposeBroadCast,
        ProposeBroadCast,
        PreProposeReply,
        ProposeReply,
        LearnerNewRequest,
        LearnerNewReply,
        Choosen
    };

    class Serializable{
    public:
         virtual std::string& Serialize() const= 0;
    };

    class PaxMessage : public Serializable{

    private:
        instance_id_t instance_id_;
        proposal_id_t proposal_id_;
        node_id_t proposer_id_;
        LogValue log_value_;

    public:
        PaxMessage(instance_id_t instance_id, proposal_id_t proposal_id, node_id_t node_id)
                : instance_id_(instance_id), proposal_id_(proposal_id), proposer_id_(node_id) {}

        instance_id_t GetInstanceId() const {
            return instance_id_;
        }

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

        node_id_t GetProposerId() const {
            return proposer_id_;
        }

        std::string &Serialize() const override{

        }
    };

    class PaxAcceptorReplyMessage : public Serializable{
        node_id_t replier_id_;
        proposal_id_t accepted_id_;
        LogValue accepted_value_;
        proposal_id_t promised_id_;
        node_id_t promised_node_id_;

    private:
        bool is_rejected_;
        node_id_t proposer_id_;

    public:
        void SetIsRejected(bool is_rejected) {
            is_rejected_ = is_rejected;
        }

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

        bool IsRejected() const {
            return is_rejected_;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }

        void SetProposerId(node_id_t proposer_id) {
            proposer_id_ = proposer_id;
        }

        void SetReplierId(node_id_t replier_id) {
            replier_id_ = replier_id;
        }
        void SetAcceptedId(proposal_id_t accepted_id) {
            accepted_id_ = accepted_id;
        }

        void SetAcceptedValue(const LogValue &accepted_value) {
            accepted_value_ = accepted_value;
        }

        void SetPromisedId(proposal_id_t promised_id) {
            promised_id_ = promised_id;
        }
        void SetPromisedNodeId(node_id_t promised_node_id) {
            promised_node_id_ = promised_node_id;
        }

        std::string &Serialize() const override {
            // todo: serialize
        }
    };
}

#endif //PAXOSME_MESSAGES_PAX_HPP
