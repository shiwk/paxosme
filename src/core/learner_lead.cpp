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
        PaxMessage message(GetNodeId(), MessageType::kMSG_TELL_INSTANCE_ID);
        message.SetInstanceId(instance_id);
        message.SetLeaderInstanceId(GetInstanceId());

        // todo II : should send my checkpoint if much lower than me.
        SendMessage(message, node_id);
    }


    void PaxLearner::HandleShallILearn(const PaxMessage &pax_message) {

        SetPossibleHigherInstanceId(pax_message.GetInstanceId());

        // todo II: be careful race condition for learner state
        if ( // todo II : learner is lower than my checkpoint, so it is much lower than me
            // ||
                pax_message.GetInstanceId() + 1 != GetInstanceId()// or learner is not catching me
                ) {

            TellInstanceId(GetInstanceId(), pax_message.GetSender());
            return;
        }

        if (pax_message.GetInstanceId() + 1 == GetInstanceId()) {
            //  learner is just following me, and in this case return the value directly.
            SendLearnedValue(pax_message.GetInstanceId(), pax_message.GetSender(), false);
        }
    }

    void PaxLearner::HandleConfirmLearn(const PaxMessage &pax_message) {
        MakeSenderReady(pax_message.GetSender(), pax_message.GetInstanceId()); // make sending loop continue work
    }

    void PaxLearner::SendLearnedValue(instance_id_t instanceId, node_id_t toNodeId, bool sync) {
        PaxosState paxosState = ReadState(instanceId);

        PaxMessage pax_message(toNodeId,
                               sync ? MessageType::kMSG_LEARNER_VALUE_SYNC : MessageType::kMSG_LEARNER_SEND_VALUE);
        pax_message.SetInstanceId(instanceId);
        pax_message.SetLearnedValue(LogValue(paxosState.accepted_value()));
        pax_message.SetAcceptedId(paxosState.accepted_proposal_id());
        pax_message.SetProposer(paxosState.proposer_id());

        SendMessage(pax_message, toNodeId);
    }

    void PaxLearner::WaitForReady() {
//        std::unique_lock<std::mutex> lck(mutex_send_);

//        if (job_status_ == LearnerSendingJobStatus::kPrepared) {
//            job_status_ = LearnerSendingJobStatus::kSending;
//            return;
//        }
        lock_.Lock();
        lock_.Wait([&]() { return job_status_ == LearnerSendingJobStatus::kPrepared; });
        job_status_ = LearnerSendingJobStatus::kSending;
        lock_.UnLock();
    }

    void PaxLearner::SendLearnedValues(instance_id_t begin_instance_id, node_id_t receiver) {
        instance_id_t to_send = begin_instance_id;
        SteadyTime timeBegin = STEADY_TIME_NOW;

        if (receiver_ != receiver)
            return; // receiver not matched

        while (to_send < GetInstanceId()) {

            SendLearnedValue(to_send, receiver, true);
            instance_id_t last_send = to_send;
            to_send++;

//            if(last_send - ack_send_ < LEAD_FOLLOW_DIS) {
//                std::unique_lock<std::mutex> lck(mutex_send_);
//                cond_v_.wait_for(lck, Time::MS(100)); // timeout or ACK notification
//            }

//            std::unique_lock<std::mutex> lck(mutex_send_);
            lock_.Lock();
            while (last_send - ack_send_ > LEAD_FOLLOW_DIS) {
                SteadyTime timeNow = STEADY_TIME_NOW;
                if (Time::StopWatchMS(timeBegin, timeNow) > SENDING_INTERNAL_MS) {
                    lock_.UnLock();
                    return; // sync too slow
                }
                lock_.WaitFor(Time::MS(100)); // timeout or ACK notification
            }

            lock_.UnLock();
        }
    }

    void PaxLearner::ClearSendingState() {
//        std::unique_lock<std::mutex> lck(mutex_send_);
        lock_.Lock();
        job_status_ = LearnerSendingJobStatus::kStale;
        begin_instance_id_ = INSTANCE_DUMMY;
        receiver_ = NODE_DUMMY;
        ack_send_ = INSTANCE_DUMMY;
        lock_.UnLock();
    }

    void PaxLearner::MakeSenderReady(node_id_t receiver, instance_id_t follower_instance_id) {
//        std::unique_lock<std::mutex> lck(mutex_send_);
        lock_.Lock();
        if (job_status_ != LearnerSendingJobStatus::kStale) {
            lock_.UnLock();
            return; // don't change if already
        }
        job_status_ = LearnerSendingJobStatus::kPrepared;
        receiver_ = receiver;
        begin_instance_id_ = follower_instance_id;
        lock_.NotifyOne(); // tell sending loop continue
        lock_.UnLock();
    }


    void PaxLearner::LearnFromSelf(const PaxMessage &pax_message) {
        if (pax_message.GetInstanceId() != GetInstanceId())
            return; // instance id not matched
        if (pax_message.GetProposer() != GetNodeId())
            return; // proposer not matched

        auto proposal = GetAcceptedProposal();
        if (proposal.proposal_id == PROPOSAL_DUMMY || pax_message.GetProposalId() != proposal.proposal_id)
            return; // proposalId == PROPOSAL_DUMMY means not accepted yet

        node_id_t node_id = proposal.node_id;
        if (node_id != GetNodeId())
            return;

        const LogValue &log_value = GetAccepted();

        // no need to persist since acceptor should have done
        LearnNew(log_value, pax_message.GetInstanceId(), proposal.proposal_id, node_id);
    }

    void PaxLearner::TellOthers(proposal_id_t proposal_id, node_id_t proposer, const LogValue &log_value) {
        PaxMessage message(GetNodeId(), MessageType::kMSG_VALUE_CHOSEN_BROADCAST);
        message.SetInstanceId(GetInstanceId());
        message.SetProposalId(proposal_id);
        message.SetProposer(proposer);
        message.SetChosenValue(log_value);

        BroadCastMessage(message);
    }

    void PaxLearner::LearnNew(const LogValue &value, instance_id_t instance_id, proposal_id_t proposal_id,
                              node_id_t proposer, bool writeState) {
        learner_state_.LearnNew(value, instance_id, proposal_id, proposer);

        if (writeState) {
            PaxosState paxos_state;
            paxos_state.set_promised_proposal_id(proposal_id);
            paxos_state.set_promised_node_id(proposer);
            paxos_state.set_instance_id(instance_id);
            paxos_state.set_accepted_proposal_id(proposal_id);
            paxos_state.set_accepted_node_id(proposer);
            paxos_state.set_accepted_value(value);

            WriteState(paxos_state);
        }
    }

    void PaxLearner::HandleValueAck(const PaxMessage &message) {
//        std::unique_lock<std::mutex> lck(mutex_send_);
        lock_.Lock();
        if (job_status_ != LearnerSendingJobStatus::kSending || receiver_ != message.GetSender()) {
            lock_.UnLock();
            return; // ignore if I am not sending or receiver already changed (maybe ack too late)
        }

        if (message.GetInstanceId() > ack_send_) {
            ack_send_ = message.GetInstanceId();
            lock_.NotifyOne(); // tell sending job continue
        }

        lock_.UnLock();
    }
}