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
        kNone = 0,

        // for acceptor
        kMSG_PROPOSE_BROADCAST = 0x1 << 3,
        kMSG_ACCEPT_BROADCAST,

        // for proposer
        kMSG_PROPOSE_ACK = 0x11 << 3,
        kMSG_ACCEPT_ACK,

        // for learner
        kMSG_VALUE_CHOSEN = 0x111 << 3,
        kMSG_SHALL_I_LEARN,
        kMSG_CONFIRM_LEARN,
        kMSG_LEARNER_VALUE_SEND,
        kMSG_LEARNER_VALUE_SYNC,
        kMSG_SYNC_VALUE_ACK,
        kMSG_VALUE_CHOSEN_BROADCAST,
        kMSG_TELL_INSTANCE_ID,

        // msg for placeholder
        kPLACEHOLDER_NEW_VALUE_COMMITTED = 0x1111 << 3
    };

//    bool MessageForProposer(MessageType message_type) {
//        return message_type >> 4 == 0x1;
//    }

    class PaxMessage {

    private:
        LogValue proposed_log_value_;
        LogValue accepted_value_;
        LogValue learned_value_;
        LogValue chosen_value_;

        proposal_id_t promisedProposalId_{};
        node_id_t promisedNodeId_{};
        instance_id_t instanceId_{};
        proposal_id_t proposalId_{};
        node_id_t proposingNodeId_{};
        node_id_t followingNodeId_{};
        MessageType messageType_;
        proposal_id_t acceptedProposalId_{};
        instance_id_t leaderInstanceId_{};
        node_id_t senderId_{};

        bool rejected_{};
    public:

        PaxMessage(node_id_t sender_id, MessageType message_type) : senderId_(sender_id),
                                                                    messageType_(message_type) {}

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

        node_id_t GetSender() const {
            return senderId_;
        }

        void SetSender(node_id_t sender_id) {
            senderId_ = sender_id;
        }

        instance_id_t GetLeaderInstanceId() const {
            return leaderInstanceId_;
        }

        void SetLeaderInstanceId(instance_id_t leaderInstanceId) {
            leaderInstanceId_ = leaderInstanceId;
        }

        bool IsRejected() const {
            return rejected_;
        }

        void SetRejected(bool rejected) {
            rejected_ = rejected;
        }

        void SetProposingNodeId(node_id_t proposer) {
            proposingNodeId_ = proposer;
        }

        proposal_id_t GetAcceptedProposal() const {
            return acceptedProposalId_;
        }

        void SetAcceptedProposal(proposal_id_t accepted_id) {
            acceptedProposalId_ = accepted_id;
        }

        const LogValue &GetAcceptedValue() const {
            return accepted_value_;
        }

        void SetAcceptedValue(const LogValue &accepted_value) {
            accepted_value_ = accepted_value;
        }

        proposal_id_t GetPromisedProposalId() const {
            return promisedProposalId_;
        }

        void SetPromisedProposalId(proposal_id_t promised_id) {
            promisedProposalId_ = promised_id;
        }

        node_id_t GetPromisedNodeId() const {
            return promisedNodeId_;
        }

        void SetPromisedNodeId(node_id_t promised_node_id) {
            promisedNodeId_ = promised_node_id;
        }

        MessageType GetMessageType() const {
            return messageType_;
        }

        void SetMessageType(MessageType message_type) {
            messageType_ = message_type;
        }

        node_id_t GetFollowingNodeId() const {
            return followingNodeId_;
        }

        void SetFollowingNodeId(node_id_t following_node_id) {
            followingNodeId_ = following_node_id;
        }

        void SetInstanceId(instance_id_t instance_id) {
            instanceId_ = instance_id;
        }


        instance_id_t GetInstanceId() const {
            return instanceId_;
        }

        void SetProposedLogValue(const LogValue &log_value) {
            proposed_log_value_ = log_value;
        }

        const LogValue &GetProposedLogValue() const {
            return proposed_log_value_;
        }

        proposal_id_t GetProposalId() const {
            return proposalId_;
        }

        void SetProposalId(proposal_id_t proposal_id) {
            proposalId_ = proposal_id;
        }

        node_id_t GetProposingNodeId() const {
            return proposingNodeId_;
        }
    };
}

#endif //PAXOSME_MESSAGES_PAX_HPP
