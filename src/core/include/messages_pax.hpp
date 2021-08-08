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
        kNone,
        kPrepareBroadCast,
        kProposeBroadCast,
        kPrepareReply,
        kProposeReply,
        kPublishChosenValue,
        kLearnerNewRequest,
        kLearnerNewReply,
        kSendValue,
        kChosen,
        kTellInstanceId
    };

    class Serializable {
    public:
        virtual std::string &Serialize() const = 0;
    };

    class PaxMessage {

    private:
        LogValue accepted_value_;
        LogValue learned_value_;
    public:
        const LogValue &GetLearnedValue() const {
            return learned_value_;
        }

        void SetLearnedValue(const LogValue &learnedValue) {
            learned_value_ = learnedValue;
        }

    private:
        proposal_id_t promised_id_;
        node_id_t promised_node_id_;
        node_id_t generator_id_;
        instance_id_t instance_id_;
        instance_id_t confirmed_instance_id_;
        proposal_id_t proposal_id_;
        node_id_t proposer_;
        LogValue proposed_log_value_;
        node_id_t following_node_id_;
        MessageType message_type_;
        proposal_id_t accepted_id_;
        bool rejected_;

    public:
        instance_id_t GetConfirmedInstanceId() const {
            return confirmed_instance_id_;
        }

        void SetConfirmedInstanceId(instance_id_t confirmed_instance_id) {
            confirmed_instance_id_ = confirmed_instance_id;
        }

        bool IsRejected() const {
            return rejected_;
        }

        void SetRejected(bool rejected) {
            rejected_ = rejected;
        }

        void SetProposer(node_id_t proposer) {
            proposer_ = proposer;
        }

        proposal_id_t GetAcceptedId() const {
            return accepted_id_;
        }

        void SetAcceptedId(proposal_id_t accepted_id) {
            accepted_id_ = accepted_id;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }

        void SetAcceptedValue(const LogValue &accepted_value) {
            accepted_value_ = accepted_value;
        }

        proposal_id_t GetPromisedId() const {
            return promised_id_;
        }

        void SetPromisedId(proposal_id_t promised_id) {
            promised_id_ = promised_id;
        }

        node_id_t GetPromisedNodeId() const {
            return promised_node_id_;
        }

        void SetPromisedNodeId(node_id_t promised_node_id) {
            promised_node_id_ = promised_node_id;
        }

        MessageType GetMessageType() const {
            return message_type_;
        }

        void SetMessageType(MessageType message_type) {
            message_type_ = message_type;
        }

        node_id_t GetFollowingNodeId() const {
            return following_node_id_;
        }

        void SetFollowingNodeId(node_id_t following_node_id) {
            following_node_id_ = following_node_id;
        }

        node_id_t GetGeneratorId() const {
            return generator_id_;
        }

        void SetGeneratorId(node_id_t self_id) {
            generator_id_ = self_id;
        }

        void SetInstanceId(instance_id_t instance_id) {
            instance_id_ = instance_id;
        }

        PaxMessage(node_id_t node_id, MessageType message_type) : generator_id_(node_id), message_type_(message_type) {}


        instance_id_t GetInstanceId() const {
            return instance_id_;
        }

        void SetProposedLogValue(const LogValue &log_value) {
            proposed_log_value_ = log_value.GetValue();
        }

        const LogValue &GetProposedLogValue() const {
            return proposed_log_value_;
        }

        proposal_id_t GetProposalId() const {
            return proposal_id_;
        }

        void SetProposalId(proposal_id_t proposal_id) {
            proposal_id_ = proposal_id;
        }

        node_id_t GetProposer() const {
            return proposer_;
        }
    };
}

#endif //PAXOSME_MESSAGES_PAX_HPP
