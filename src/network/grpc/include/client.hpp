//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_CLIENT_HPP
#define PAXOSME_CLIENT_HPP

#include "paxosme.grpc.pb.h"
#include "network_client.hpp"

using grpc::Channel;
using paxos::Paxosme;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using ::grpc::ChannelInterface;

namespace paxosme {
    template<class Response>
    using RequestCall = std::function<std::unique_ptr<::grpc::ClientAsyncResponseReader<Response>>(
            ::grpc::ClientContext * , ::grpc::CompletionQueue * )>;

    // struct for keeping state and data information
    template<typename T>
    struct AsyncClientCall {
        // Container for the data we expect from the server.
        T reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // Storage for the status of the RPC upon completion.
        Status status;

        std::unique_ptr<grpc::ClientAsyncResponseReader<T>> response_reader;
    };


    class GrpcClient : public NetworkClient, public std::enable_shared_from_this<GrpcClient> {
    public:
        explicit GrpcClient(const std::shared_ptr<ChannelInterface> &channel)
                : stub_(Paxosme::NewStub(channel)) {}

        friend class Client;

        static GrpcClient *NewClient(const std::string &);

        bool Send(const PodMsg &) override;
    private:


        std::unique_ptr<Paxosme::Stub> stub_;

        template<class TRequest, class TResponse>
        std::unique_ptr<::grpc::ClientAsyncResponseReader<TResponse>>
        PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq, TRequest &request);

        template<class TRequest, class TResponse>
        bool Send(TRequest &);

//        Paxosme::Stub stub_;

//        template<class Request, class Response>
//        std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::paxos::AcceptReply>> PrepareAsyncFunc(::grpc::ClientContext *c,
//                                                                                                    ::grpc::CompletionQueue *cq, Request &request){
//            return stub_->PrepareAsyncAccept(request);
//        }

//        template<class Request, class Response>
//        bool AsyncCall(std::unique_ptr< ::grpc::ClientAsyncResponseReader< Response>> *f(::grpc::ClientContext *, ::grpc::CompletionQueue *), Request &r){
//    // Call object to store rpc data
//            auto *call = new AsyncClientCall<Response>;
//
//            // stub_->PrepareAsyncSayHello() creates an RPC object, returning
//            // an instance to store in "call" but does not actually start the RPC
//            // Because we are using the asynchronous API, we need to hold on to
//            // the "call" instance in order to get updates on the ongoing RPC.
//            ::grpc::CompletionQueue cq;
//            call->response_reader = f(&call->context, r, &cq);
////            call->response_reader = async_request();
//            // StartCall initiates the RPC call
//            call->response_reader->StartCall();
//            // Request that, upon completion of the RPC, "reply" be updated with the
//            // server's response; "status" with the indication of whether the operation
//            // was successful. Tag the request with the memory address of the call
//            // object.
//            call->response_reader->Finish(&call->reply, &call->status, (void *) call);
//
//            bool res = false;
//            if (call->status.ok()) {
//                r = call->reply;
//                res = true;
//            }
//            delete call;
//            return res;
//        }
    public:
        // code reference https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/greeter_async_client2.cc
        template<class TRequest, class TResponse>
        bool AsyncCall(TRequest request, TResponse &response) {

//            RequestCall<TResponse> async_request = MakePrepareRequest<TRequest, TResponse>(request,kPROPOSE);

            // Call object to store rpc data
            auto *call = new AsyncClientCall<TResponse>;

            // stub_->PrepareAsyncSayHello() creates an RPC object, returning
            // an instance to store in "call" but does not actually start the RPC
            // Because we are using the asynchronous API, we need to hold on to
            // the "call" instance in order to get updates on the ongoing RPC.
            ::grpc::CompletionQueue cq;
            call->response_reader = PrepareRequest<TRequest, TResponse>(&call->context, &cq, request);
//            call->response_reader = async_request();
            // StartCall initiates the RPC call
            call->response_reader->StartCall();
            // Request that, upon completion of the RPC, "reply" be updated with the
            // server's response; "status" with the indication of whether the operation
            // was successful. Tag the request with the memory address of the call
            // object.

            call->response_reader->Finish(&call->reply, &call->status, (void *) 1);

            void *got_tag;
            bool ret = false;

            bool next = cq.Next(&got_tag, &ret);
            GPR_ASSERT(next);
            GPR_ASSERT(ret && got_tag == (void *) 1);

            if (call->status.ok()) {
                response = call->reply;
                delete call;
                return true;
            }

            delete call;
            return false;
        }
    };

    template<class TRequest, class TResponse>
    bool GrpcClient::Send(TRequest &request) {
        TResponse response;
        return AsyncCall(request, response);
    }
}
#endif //PAXOSME_CLIENT_HPP
