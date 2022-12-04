//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_LEARNER_PAX_HPP
#define PAXOSME_LEARNER_PAX_HPP

#include <log_value.hpp>
#include "player_pax.hpp"
#include "config.hpp"
#include "lock.hpp"
#include <thread>
#include <condition_variable>
#include <time.hpp>
#include <future>

#define SHALL_LEARN_TIMEOUT_CONST 3000
#define LEAD_FOLLOW_DIS 10
#define SENDING_INTERNAL_MS 5000

namespace paxosme {
    class PaxLearnerState {
        LogValue log_value_;
        bool learned_;
        PaxosOptions *config_;

    public:
        explicit PaxLearnerState(const PaxosOptions *config) : config_(const_cast<PaxosOptions *>(config)), learned_(false) {}

        void LearnNew(const LogValue &log_value, instance_id_t instance_id, proposal_id_t proposal_id,
                      node_id_t proposer_node_id) {
            log_value_ = log_value;
            learned_ = true;
        }

        const LogValue &GetLearnedValue() {
            return log_value_;
        }

        bool Learned() const {
            return learned_;
        }

        void Reset() {
            log_value_.clear();
            learned_ = false;
        }
    };


    class PaxLearner : public PaxPlayer {
    public:
        PaxLearner(const PaxosOptions *, const PaxCommunicator *, const PaxStorage *);

    public:
        void HandleMessage(const PaxMessage &) override;

        void NewInstance() override;

        bool Learned() const;

        void Init(const PaxController *controller);

        // follow
        void ShallLearn(); // request learn from others
        const LogValue &GetLearnedValue();

        bool AnymoreToLearn();

    private:
        void LearnFromSelf(const PaxMessage &);

        PaxLearnerState learner_state_;

    private:

        /// follow
        void TellInstanceId(instance_id_t, node_id_t); // tell others current instance_id

        instance_id_t highest_known_instance_id_;

        void SetPossibleHigherInstanceId(const instance_id_t &);

        void LearnNew(const LogValue &, instance_id_t, proposal_id_t, node_id_t proposer, bool writeState = false);

        bool is_learning_; // // exactly learning from one node
        std::mutex mutex_follow_;

        void ConfirmLearn(node_id_t node_id);

        void LearnFromOthers(const PaxMessage &);

        bool HandleSenderPublish(const PaxMessage &);

        void HandleOthersPublish(const PaxMessage &pax_message);
        void HandleSyncValue(const PaxMessage &pax_message);

        void HandleConfirmLearn(const PaxMessage &);

        void HandleTellNewInstanceId(const PaxMessage &);


        /// lead
        void HandleShallILearn(const PaxMessage &);

        void HandleValueAck(const PaxMessage &);

        enum LearnerSendingJobStatus {
            kStale,
            kPrepared,
            kSending
        };

        LearnerSendingJobStatus job_status_;
        node_id_t receiver_;
        instance_id_t begin_instance_id_;
//        std::mutex mutex_send_;
//        std::condition_variable_any cond_v_;
        MyLock lock_;
        bool is_sending_;
        instance_id_t ack_send_;
        std::future<void> learner_send_loop_;

        [[noreturn]] void SendingLoop();

        void WaitForReady();

        void ClearSendingState();

        void SendLearnedValues(instance_id_t begin_instance_id, node_id_t receiver);

        void MakeSenderReady(node_id_t receiver, instance_id_t follower_instance_id);

        void SendLearnedValue(instance_id_t, node_id_t, bool sync);

        void TellOthers(proposal_id_t proposal_id, node_id_t proposer, const LogValue &log_value);

        void AckLearnValue(node_id_t node_id);

        static int shall_Learn_delay_;
    };
}


#endif //PAXOSME_LEARNER_PAX_HPP
