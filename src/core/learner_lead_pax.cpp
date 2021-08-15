//
// Created by k s on 2021/8/15.
//
#include <learner_pax.hpp>

namespace paxosme {
    [[noreturn]] void PaxLearner::SendingLoop() {
        while (true) {
            WaitForReady();
            SendLearnedValues(begin_instance_id_, receiver_);
            ClearSendingState();
        }
    }

    void PaxLearner::TellInstanceId(const instance_id_t instance_id, const node_id_t node_id) {
        PaxMessage message(GetNodeId(), MessageType::kTellInstanceId);
        message.SetInstanceId(instance_id);
        message.SetLeaderInstanceId(GetInstanceId());

        // todo II : should send my checkpoint if much lower than me.
        SendMessage(message, node_id);
    }


    void PaxLearner::HandleShallILearn(const PaxMessage &pax_message) {
        SetPossibleHigherInstanceId(pax_message.GetInstanceId());

        // todo II: be careful race condition for learner state
        if ( // todo I : learner is lower than my checkpoint, so it is much lower than me
        // ||
        pax_message.GetInstanceId() + 1 != GetInstanceId()// or learner is not catching me
        ) {

            TellInstanceId(GetInstanceId(), pax_message.GetSenderId());
            return;
        }

        if (pax_message.GetInstanceId() + 1 == GetInstanceId()) {
            //  learner is just following me, and in this case return the value directly.
            SendLearnedValue(pax_message.GetInstanceId(), pax_message.GetSenderId());
        }
    }

    void PaxLearner::HandleConfirmLearn(const PaxMessage &pax_message) {
        MakeReady(pax_message.GetSenderId());
    }

    void PaxLearner::SendLearnedValue(instance_id_t instanceId, node_id_t toNodeId) {
        PaxosState paxosState = ReadState(instanceId);

        PaxMessage pax_message(toNodeId, MessageType::kSendValue);
        pax_message.SetInstanceId(instanceId);
        pax_message.SetLearnedValue(LogValue(paxosState.accepted_value()));
        pax_message.SetAcceptedId(paxosState.accepted_proposal_id());
        pax_message.SetProposer(paxosState.proposer_id());
        SendMessage(pax_message, toNodeId);
    }

    void PaxLearner::WaitForReady() {
        std::unique_lock<std::mutex> lck(mutex_send_);
        cond_v_.wait(lck, [&]() { return job_status_ == LearnerSendingJobStatus::kReady; });
        is_sending_ = true;
    }

    void PaxLearner::SendLearnedValues(instance_id_t begin_instance_id, node_id_t receiver) {
        instance_id_t to_send = begin_instance_id;
        uint64_t timeBegin = Time::NowSinceEpochInMS();

        std::unique_lock<std::mutex> lck(mutex_send_);
        if (receiver_ != receiver)
            return; // receiver not matched

        while (to_send < GetInstanceId()) {
            SendLearnedValue(to_send, receiver);
            instance_id_t last_send = to_send;
            to_send++;
            while (last_send - ack_send_ < 10) {
                uint64_t timeNow = Time::NowSinceEpochInMS();
                if (timeNow - timeBegin > 5000)
                    return; // sync too slow
                cond_v_.wait_for(lck, Time::MS(10));
            }

            // big gap between last_send and ack_send_
            return;
        }
    }

    void PaxLearner::ClearSendingState() {
        std::unique_lock<std::mutex> lck(mutex_send_);

        job_status_ = LearnerSendingJobStatus::kStale;
        begin_instance_id_ = -1;
        receiver_ = -1;
        is_sending_ = false;
        ack_send_ = -1;
    }

    void PaxLearner::MakeReady(node_id_t receiver) {
        std::unique_lock<std::mutex> lck(mutex_send_);
        if (job_status_ == LearnerSendingJobStatus::kReady || is_sending_)
            return;

        job_status_ = LearnerSendingJobStatus::kReady;
        receiver_ = receiver;
    }
}