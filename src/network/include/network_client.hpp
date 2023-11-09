//
// Created by kk on 2023/2/21.
//

#ifndef PAXOSME_NETWORK_CLIENT_HPP
#define PAXOSME_NETWORK_CLIENT_HPP

#include <common.hpp>
#include <network.hpp>

namespace paxosme {
    enum PodMsgType {
        kPod_None = 0,
        // for acceptor
        kPOD_MSG_PROPOSE_BROADCAST = 0x1 << 3,
        kPOD_MSG_ACCEPT_BROADCAST,
        // for proposer
        kPOD_MSG_PROPOSE_ACK = 0x11 << 3,
        kPOD_MSG_ACCEPT_ACK,
        // for learner
        kPOD_MSG_VALUE_CHOSEN = 0x111 << 3,
        kPOD_MSG_SHALL_I_LEARN,
        kPOD_MSG_CONFIRM_LEARN,
        kPOD_MSG_LEARNER_VALUE_SEND,
        kPOD_MSG_LEARNER_VALUE_SYNC,
        kPOD_MSG_SYNC_VALUE_ACK,
        kPOD_MSG_VALUE_CHOSEN_BROADCAST,
        kPOD_MSG_TELL_INSTANCE_ID,
        // msg for placeholder
        kPod_PLACEHOLDER_NEW_VALUE_COMMITTED = 0x1111 << 3
    };

    struct PodMsg {
    public:
        int messageType;
        LogValue proposedLogValue;
        LogValue acceptedValue;
        LogValue learnedValue;
        LogValue chosenValue;
        proposal_id_t promisedProposalId;
        node_id_t promisedNodeId;
        instance_id_t instanceId;
        proposal_id_t proposalId;
        node_id_t proposingNodeId;
        node_id_t followingNodeId;
        proposal_id_t acceptedProposalId;
        instance_id_t leaderInstanceId;
        node_id_t senderId;
        bool rejected;
    };

    class NetworkClient {
    public:
        static NetworkClient* New(const Peer &);

        virtual bool Send(const PodMsg &) = 0;
    };
}

#endif //PAXOSME_NETWORK_CLIENT_HPP
