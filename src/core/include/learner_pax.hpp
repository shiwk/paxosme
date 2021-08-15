//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_LEARNER_PAX_HPP
#define PAXOSME_LEARNER_PAX_HPP

#include <log_value.hpp>
#include "player_pax.hpp"
#include "config_pax.hpp"
#include <thread>
#include <condition_variable>
#include <Time.hpp>
#include <future>

namespace paxosme {
    class PaxLearnerState {
        LogValue log_value_;
        instance_id_t learned_instance_id_;
    public:
        instance_id_t GetLearnedInstanceId() const {
            return learned_instance_id_;
        }

        void SetLearnedInstanceId(instance_id_t learnedInstanceId) {
            learned_instance_id_ = learnedInstanceId;
        }

    private:
        proposal_id_t proposal_id_;
        node_id_t proposer_;
    public:
        void LearnNew(const LogValue &log_value, instance_id_t instance_id, proposal_id_t proposal_id,
                      node_id_t proposer_node_id) {
            log_value_ = log_value;
            learned_instance_id_ = instance_id;
            proposal_id_ = proposal_id;
            proposer_ = proposer_node_id;
        }

        const LogValue &GetLearnedValue() {
            return log_value_;
        }
    };


    class PaxLearner : public PaxPlayer {
    public:
        void ShallILearn(); // request learn from others
        void HandleShallILearn(const PaxMessage &);

        void HandleTellNewInstanceId(const PaxMessage&);

        void HandleConfirmLearn(const PaxMessage&);

        void LearnBySelf(const PaxMessage &pax_message);

        const LogValue &GetLearnedValue();

        bool AnymoreToLearn();

        void Init();

        void SendLearnedValue(instance_id_t, node_id_t);


    private: // follow
        void TellOtherLearners();

        void TellInstanceId(instance_id_t, node_id_t); // tell others current instance_id

        PaxLearnerState *learner_state_;
        instance_id_t highest_known_instance_id_;

        void SetPossibleHigherInstanceId(const instance_id_t &);

        void LearnNew(const LogValue &, instance_id_t, proposal_id_t, node_id_t proposer, bool writeState);

        bool is_learning_; // // exactly learning from one node
        std::mutex mutex_follow_;
        void ConfirmLearn(node_id_t node_id);

    private: // lead
        enum LearnerSendingJobStatus{
            kStale,
            kPrepared,
            kReady
        };

        LearnerSendingJobStatus job_status_;
        node_id_t receiver_;
        instance_id_t begin_instance_id_;
        std::mutex mutex_send_;
        std::condition_variable_any cond_v_;
        bool is_sending_;
        instance_id_t ack_send_;
        std::future <void> learner_send_loop_;

        [[noreturn]] void SendingLoop();
        void WaitForReady();
        void ClearSendingState();
        void SendLearnedValues(instance_id_t begin_instance_id, node_id_t receiver);
        void MakeReady(node_id_t receiver);
    };
}


#endif //PAXOSME_LEARNER_PAX_HPP
