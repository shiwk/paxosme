
//
// Created by shiwk on 2021/3/11.
//

#include <client.hpp>

namespace paxosme {
    bool GrpcClient::Prepare(const PaxMessage &pax_message) {
        paxos::PrepareRequest prepare_request;
        prepare_request.set_instance_id(pax_message.GetInstanceId());
        prepare_request.set_proposal_id(pax_message.GetProposalId());
        prepare_request.set_proposer_id(pax_message.GetProposer());

        auto async_request = [&](::grpc::ClientContext *c,
                                 ::grpc::CompletionQueue *cq) {
            return stub_->PrepareAsyncPrepare(c, prepare_request, cq);
        };
        AsyncCall<paxos::PrepareReply>(async_request);
        return true;
    }

    bool GrpcClient::Propose(const PaxMessage &pax_message) {
        paxos::ProposeRequest propose_request;
        propose_request.set_proposer_id(pax_message.GetProposer());
        propose_request.set_proposal_id(pax_message.GetProposalId());
        propose_request.set_instance_id(pax_message.GetInstanceId());
        propose_request.set_proposed_log_value(pax_message.GetProposedLogValue());

        auto async_request = [&](::grpc::ClientContext *c,
                                ::grpc::CompletionQueue *cq) {
            return stub_->PrepareAsyncPropose(c, propose_request, cq);
        };
        AsyncCall<paxos::ProposeReply>(async_request);
        return false;
    }
}
