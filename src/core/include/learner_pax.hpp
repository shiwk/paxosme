//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_LEARNER_PAX_HPP
#define PAXOSME_LEARNER_PAX_HPP

#include <log_value.hpp>
#include "player_pax.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class PaxLearnerState {
        LogValue log_value_;
        instance_id_t instance_id_;
        proposal_id_t proposal_id_;
        node_id_t proposer_;
    public:
        void LearnNew(const LogValue &log_value, instance_id_t instance_id, proposal_id_t proposal_id,
                      node_id_t proposer_node_id) {
            log_value_ = log_value;
            instance_id_ =instance_id;
            proposal_id_ = proposal_id;
            proposer_ = proposer_node_id
        }

        const LogValue &GetLearnedValue() {
            return log_value_;
        }
    };

    class NewValueRequest : public Serializable {
    public:
        NewValueRequest(node_id_t node_id, node_id_t following_node_id, instance_id_t instance_id) :
        node_id_(node_id),following_node_id_(following_node_id),instance_id_(instance_id) {}

        const node_id_t &GetNodeId() const {
            return node_id_;
        }

        const instance_id_t &GetInstanceId() const {
            return instance_id_;
        }

        const node_id_t &GetFollowingNodeId() const {
            return following_node_id_;
        }

        std::string &Serialize() const override {
        }

    private:
        node_id_t node_id_;
        node_id_t following_node_id_;
        instance_id_t instance_id_;
    };

    class PaxLearner : public PaxPlayer {
    public:
//        void HandleNewValueProposed(const PaxMessage &pax_message);

        void LearnFromOthers(const PaxMessage &pax_message);

        void RequestLearning(); // request learn from others
        void HandleLearningRequest(const NewValueRequest &new_value_request);

//        void HandleReplyLearning(const PaxMessage &pax_message);

        void HandleTellNewInstanceId(instance_id_t instance_id, node_id_t node_id);

        void LearnBySelf(const PaxMessage &pax_message);

        const LogValue &GetLearnedValue();

        bool AnymoreToLearn();

    private:
//        void ReplyLearnedValue(node_id_t node_id, LogValue log_value); // HandleLearnRequest
//        void BroadCastLearnedValue(LogValue log_value); // if chosen my local proposer
        void TellOtherLearners();

        void TellInstanceId(instance_id_t instance_id, node_id_t node_id); // tell others current instance_id

        void ReplyLearning(instance_id_t instance_id); // reply learn request

        PaxLearnerState *learner_state_;
        instance_id_t highest_known_instance_id_;

        void SetPossibleHigherInstanceId(const instance_id_t &instance_id);

        // send instance id to others
//        void SendInstanceId(instance_id_t instance_id, const node_id_t node_id);
//        void SendFollowers(const PaxMessage &message, MessageType request_type);
        void LearnNew(const LogValue &value, instance_id_t instance_id, proposal_id_t proposal_id, node_id_t proposer, bool writeState);
    };
}


#endif //PAXOSME_LEARNER_PAX_HPP
