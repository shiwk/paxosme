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

#define SHALLILEARN_TIMEOUT_CONST 3000

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

    public:
        void LearnNew(const LogValue &log_value, instance_id_t instance_id, proposal_id_t proposal_id,
                      node_id_t proposer_node_id) {
            log_value_ = log_value;
            learned_instance_id_ = instance_id;
        }

        const LogValue &GetLearnedValue() {
            return log_value_;
        }

        void Reset(){
            log_value_ = "";
            learned_instance_id_ = 0;
        }
    };


    class PaxLearner : public PaxPlayer {
    public:
        // follow
        void ShallILearn(); // request learn from others

        void HandleTellNewInstanceId(const PaxMessage&);

        void HandleConfirmLearn(const PaxMessage&);

        const LogValue &GetLearnedValue();

        bool AnymoreToLearn();

        void Init();
        bool HandleSenderPublish(const PaxMessage &);
        void HandleOthersPublish(const PaxMessage &pax_message);
        bool Learned();

        void NewInstance() override;

    public:
        // lead
        void HandleShallILearn(const PaxMessage &);
        void HandleValueAck(const PaxMessage &);

    private:
        void LearnFromSelf(const PaxMessage &);

    private:
        // follow
        void TellInstanceId(instance_id_t, node_id_t); // tell others current instance_id

        PaxLearnerState *learner_state_;
        instance_id_t highest_known_instance_id_;

        void SetPossibleHigherInstanceId(const instance_id_t &);

        void LearnNew(const LogValue &, instance_id_t, proposal_id_t, node_id_t proposer, bool writeState = false);

        bool is_learning_; // // exactly learning from one node
        std::mutex mutex_follow_;
        void ConfirmLearn(node_id_t node_id);
        void LearnFromOthers(const PaxMessage &);

    private: // lead
        enum LearnerSendingJobStatus{
            kStale,
            kPrepared,
            kSending
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
        void MakeReady(node_id_t receiver, instance_id_t i);
        void SendLearnedValue(instance_id_t, node_id_t, bool syn = false);
        void TellFollowers(proposal_id_t proposal_id, node_id_t node_id, const LogValue& value);
        void Ack(node_id_t id);

        static int SHALLILEARN_DELAY;
    };
}


#endif //PAXOSME_LEARNER_PAX_HPP
