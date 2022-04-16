
//
// Created by shiwk on 2021/3/11.
//

#include <client.hpp>

template<class TResponse>
using ResponseReader = std::unique_ptr<::grpc::ClientAsyncResponseReader<TResponse>>;

namespace paxosme {

    template<>
    ResponseReader<paxos::ProposeReply>
    GrpcClient::PrepareRequest<paxos::ProposeRequest, paxos::ProposeReply>(::grpc::ClientContext *c,
                                                                           ::grpc::CompletionQueue *cq,
                                                                           paxos::ProposeRequest &request) {
        return stub_->PrepareAsyncPropose(c, request, cq);
    }

    template<>
    ResponseReader<paxos::ProposeAckReply>
    GrpcClient::PrepareRequest<paxos::ProposeAckRequest, paxos::ProposeAckReply>(::grpc::ClientContext
                                                                                 *c,
                                                                                 ::grpc::CompletionQueue *cq,
                                                                                 paxos::ProposeAckRequest
                                                                                 &request) {
        return stub_->PrepareAsyncProposeAck(c, request, cq);
    }

    template<>
    ResponseReader<paxos::AcceptReply>
    GrpcClient::PrepareRequest<paxos::AcceptRequest, paxos::AcceptReply>(::grpc::ClientContext
                                                                         *c,
                                                                         ::grpc::CompletionQueue *cq,
                                                                         paxos::AcceptRequest
                                                                         &request) {
        return stub_->PrepareAsyncAccept(c, request, cq);
    }

    template<>
    ResponseReader<paxos::AcceptAckReply>
    GrpcClient::PrepareRequest<paxos::AcceptAckRequest, paxos::AcceptAckReply>(::grpc::ClientContext
                                                                               *c,
                                                                               ::grpc::CompletionQueue *cq,
                                                                               paxos::AcceptAckRequest
                                                                               &request) {
        return stub_->PrepareAsyncAcceptAck(c, request, cq);
    }

}
