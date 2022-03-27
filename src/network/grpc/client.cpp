
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
        AsyncCall<paxos::ProposeReply>(async_request);
        return true;
    }

    bool GrpcClient::Accept(const PaxMessage &pax_message) {
        paxos::AcceptRequest acceptRequest;
        acceptRequest.set_proposer_id(pax_message.GetProposer());
        acceptRequest.set_proposal_id(pax_message.GetProposalId());
        acceptRequest.set_instance_id(pax_message.GetInstanceId());
        acceptRequest.set_proposed_log_value(pax_message.GetProposedLogValue());

        auto async_request = [&](::grpc::ClientContext *c,
                                ::grpc::CompletionQueue *cq) {
            return stub_->PrepareAsyncAccept(c, acceptRequest, cq);
        };
        AsyncCall<paxos::AcceptReply>(async_request);
        return false;
    }
}
