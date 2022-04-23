
//
// Created by shiwk on 2021/3/11.
//

#include <client.hpp>

template<class TResponse>
using ResponseReader = std::unique_ptr<::grpc::ClientAsyncResponseReader<TResponse>>;

namespace paxosme {

    template<>
    ResponseReader<paxos::ProposeReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq, paxos::ProposeRequest &request) {
        return stub_->PrepareAsyncPropose(c, request, cq);
    }

    template<>
    ResponseReader<paxos::ProposeAckReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::ProposeAckRequest &request) {
        return stub_->PrepareAsyncProposeAck(c, request, cq);
    }

    template<>
    ResponseReader<paxos::AcceptReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq, paxos::AcceptRequest &request) {
        return stub_->PrepareAsyncAccept(c, request, cq);
    }

    template<>
    ResponseReader<paxos::AcceptAckReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::AcceptAckRequest &request) {
        return stub_->PrepareAsyncAcceptAck(c, request, cq);
    }

    template<>
    ResponseReader<paxos::NewValueChosenReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::NewValueChosenRequest &request) {
        return stub_->PrepareAsyncNewValueChosen(c, request, cq);
    }

}
