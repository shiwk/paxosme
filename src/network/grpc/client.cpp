
//
// Created by shiwk on 2021/3/11.
//

#include <client.hpp>

namespace paxosme {
    bool GrpcClient::Propose(const PaxMessage &pax_message) {
        paxos::ProposeRequest proposeRequest;
        proposeRequest.set_instance_id(pax_message.GetInstanceId());
        proposeRequest.set_proposal_id(pax_message.GetProposalId());
        proposeRequest.set_proposer_id(pax_message.GetProposer());

        auto async_request = [&](::grpc::ClientContext *c,
                                 ::grpc::CompletionQueue *cq) {
            return stub_->PrepareAsyncPropose(c, proposeRequest, cq);
        };

        paxos::ProposeReply proposeReply;
        bool res = AsyncCall<paxos::ProposeReply>(async_request, proposeReply);

        if (!res)
            return false;

        // todo I: handle message
        return true;
    }

    bool GrpcClient::Accept(const PaxMessage &pax_message) {
        paxos::AcceptRequest acceptRequest;
        acceptRequest.set_proposer_id(pax_message.GetProposer());
        acceptRequest.set_proposal_id(pax_message.GetProposalId());
        acceptRequest.set_instance_id(pax_message.GetInstanceId());
        acceptRequest.set_proposed_log_value(pax_message.GetProposedLogValue());

        auto prepareRequestFunc = [&](::grpc::ClientContext *c,
                                      ::grpc::CompletionQueue *cq) {
            return stub_->PrepareAsyncAccept(c, acceptRequest, cq);
        };
        ;

        paxos::AcceptReply acceptReply;
        bool res = AsyncCall<paxos::AcceptReply>(prepareRequestFunc, acceptReply);
        if (!res)
            return false;
        // todo I: handle message
        return true;
    }

    bool GrpcClient::ProposeAck(const PaxMessage &pax_message) {
        paxos::ProposeAckRequest proposeAckRequest;
        proposeAckRequest.set_replier_id(pax_message.GetSender());
        proposeAckRequest.set_is_rejected(pax_message.IsRejected());
        proposeAckRequest.set_instance_id(pax_message.GetInstanceId());


        if (pax_message.IsRejected()) {
            proposeAckRequest.set_promised_id(pax_message.GetPromisedId());
            proposeAckRequest.set_promised_node_id(pax_message.GetPromisedNodeId());
        }

        if (!pax_message.GetAcceptedValue().empty())
            proposeAckRequest.set_accepted_log_value(pax_message.GetAcceptedValue());

        auto async_request = [&](::grpc::ClientContext *c, ::grpc::CompletionQueue *cq) {
                return stub_->PrepareAsyncProposeAck(c, proposeAckRequest, cq);
        };

        paxos::ProposeAckReply proposeAckReply;
        bool res = AsyncCall<paxos::ProposeAckReply>(async_request, proposeAckReply);

        if (!res)
            return false;

        // todo I: handle message
        return true;
    }
}
