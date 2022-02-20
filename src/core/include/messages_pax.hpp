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
        // for acceptor
        kPrepareBroadCast,
        kProposeBroadCast,

        // for proposer
        kPrepareReply,
        kProposeReply,

        // for learner
        kSenderPublishChosenValue,
        kShallILearn,
        kConfirmLearn,
        kSendValue,
        kValue_SYNC,
        kValue_ACK,

        kBroadCastChosen,
        kTellInstanceId
    };

    class Serializable {
    public:
        virtual std::string &Serialize() const = 0;
    };

    class PaxMessage {

    private:
        LogValue proposed_log_value_;
        LogValue accepted_value_;
        LogValue learned_value_;
        LogValue chosen_value_;
    public:
        const LogValue &GetChosenValue() const {
            return chosen_value_;
        }

        void SetChosenValue(const LogValue &chosen_value) {
            chosen_value_ = chosen_value;
        }

        const LogValue &GetLearnedValue() const {
            return learned_value_;
        }

        void SetLearnedValue(const LogValue &learnedValue) {
            learned_value_ = learnedValue;
        }

    private:
        proposal_id_t promised_id_{};
        node_id_t promised_node_id_{};
        instance_id_t instance_id_{};
        instance_id_t confirmed_instance_id_{};
        proposal_id_t proposal_id_{};
        node_id_t proposer_{};
        node_id_t following_node_id_{};
        MessageType message_type_;
        proposal_id_t accepted_id_{};
        instance_id_t leader_instance_id_{};
        node_id_t sender_id_{};

        bool rejected_{};

    public:
        node_id_t GetSender() const {
            return sender_id_;
        }

        void SetSender(node_id_t sender_id) {
            sender_id_ = sender_id;
        }

        instance_id_t GetLeaderInstanceId() const {
            return leader_instance_id_;
        }

        void SetLeaderInstanceId(instance_id_t leaderInstanceId) {
            leader_instance_id_ = leaderInstanceId;
        }

    public:
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

        void SetInstanceId(instance_id_t instance_id) {
            instance_id_ = instance_id;
        }

        PaxMessage() = default;
        PaxMessage(node_id_t sender_id, MessageType message_type) : sender_id_(sender_id), message_type_(message_type) {}


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
