//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_CLIENT_HPP
#define PAXOSME_CLIENT_HPP

#include <messages_pax.hpp>
#include "paxosme.grpc.pb.h"

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
    template<class R>
    using RequestCall = std::function<std::unique_ptr< ::grpc::ClientAsyncResponseReader<R>>(::grpc::ClientContext*, ::grpc::CompletionQueue*)>;

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


    class GrpcClient {
    public:
        explicit GrpcClient(const std::shared_ptr<ChannelInterface> &channel)
                : stub_(Paxosme::NewStub(channel)) {}

        bool Prepare(const PaxMessage &pax_message);

        bool Propose(const PaxMessage &pax_message);

    private:
        std::unique_ptr<Paxosme::Stub> stub_;

        // code reference https://github.com/grpc/grpc/blob/master/examples/cpp/helloworld/greeter_async_client2.cc
        template<class R>
        void AsyncCall(RequestCall<R> async_request) {
            // Call object to store rpc data
            auto* call = new AsyncClientCall<R>;

            // stub_->PrepareAsyncSayHello() creates an RPC object, returning
            // an instance to store in "call" but does not actually start the RPC
            // Because we are using the asynchronous API, we need to hold on to
            // the "call" instance in order to get updates on the ongoing RPC.
            ::grpc::CompletionQueue cq;
            call->response_reader = async_request(&call->context, &cq);
//            call->response_reader = async_request();
            // StartCall initiates the RPC call
            call->response_reader->StartCall();
            // Request that, upon completion of the RPC, "reply" be updated with the
            // server's response; "status" with the indication of whether the operation
            // was successful. Tag the request with the memory address of the call
            // object.
            call->response_reader->Finish(&call->reply, &call->status, (void*)call);

        }
    };
}
#endif //PAXOSME_CLIENT_HPP
