
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

    template<>
    ResponseReader<paxos::ShallILearnReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::ShallILearnRequest &request) {
        return stub_->PrepareAsyncShallILearn(c, request, cq);
    }

    template<>
    ResponseReader<paxos::LearnerValueSyncReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::LearnerValueSyncRequest &request) {
        return stub_->PrepareAsyncLearnerValueSync(c, request, cq);
    }

    template<>
    ResponseReader<paxos::AckSyncValueReply>
    GrpcClient::PrepareRequest(grpc::ClientContext *c, grpc::CompletionQueue *cq, paxos::AckSyncValueRequest &request) {
        return stub_->PrepareAsyncAckSyncValue(c, request, cq);
    }

    template<>
    ResponseReader<paxos::LearnerValueSendReply> GrpcClient::PrepareRequest(::grpc::ClientContext
                                                                            *c, ::grpc::CompletionQueue *cq,
                                                                            paxos::LearnerValueSendRequest &request) {
        return stub_->PrepareAsyncLearnerValueSend(c, request, cq);
    }

    template<>
    ResponseReader<paxos::TellInstanceIdReply> GrpcClient::PrepareRequest(::grpc::ClientContext
                                                                          *c,
                                                                          ::grpc::CompletionQueue *cq,
                                                                          paxos::TellInstanceIdRequest &request) {
        return stub_->PrepareAsyncTellInstanceId(c, request, cq);
    }

    template<>
    ResponseReader<paxos::ConfirmLearnReply> GrpcClient::PrepareRequest(::grpc::ClientContext
                                                                        *c,
                                                                        ::grpc::CompletionQueue *cq,
                                                                        paxos::ConfirmLearnRequest
                                                                        &request) {
        return stub_->PrepareAsyncConfirmLearn(c, request, cq);
    }
}
