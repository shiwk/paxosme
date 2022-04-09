//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_SERVER_HPP
#define PAXOSME_SERVER_HPP

#include "paxosme.grpc.pb.h"
#include <grpcpp/server_builder.h>
#include <grpcpp/server.h>

using paxos::Paxosme;
using grpc::ServerBuilder;
using grpc::Server;

class NetworkConfig;

namespace paxosme {
    class PaxosmeGrpcImpl final : public Paxosme::Service {
    public:

        grpc::Status Propose(grpc::ServerContext *context, const paxos::ProposeRequest *request,
                             paxos::ProposeReply *response) override;

        grpc::Status Accept(grpc::ServerContext *context, const paxos::AcceptRequest *request,
                            paxos::AcceptReply *response) override;

    private:

    };

    class ServerImpl {
    public:
        void Start(std::string &listening);

        ~ServerImpl();

    private:
        std::unique_ptr<Server> server_;
        paxos::Paxosme::AsyncService asyncService_;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;

        void HandleRpcs();
    };

    class BaseCallData {
    public:
        void Proceed() {
            if (status_ == CREATE) {
                CreateStatusFunc();
            } else if (status_ == PROCESS) {
                ProcessStatusFunc();
            } else {
                FinishStatusFunc();
            }
        }

    protected:
        BaseCallData(paxos::Paxosme::AsyncService *service, grpc::ServerCompletionQueue *cq) : service_(service),
                                                                                               cq_(cq),
                                                                                               status_(CREATE) {}

        virtual void InitRequestState() = 0;

        virtual void Handle() = 0;

        // The means of communication with the gRPC runtime for an asynchronous
        // server.
        paxos::Paxosme::AsyncService *service_;
        // The producer-consumer queue where for asynchronous server notifications.
        grpc::ServerCompletionQueue *cq_;
        // Context for the rpc, allowing to tweak aspects of it such as the use
        // of compression, authentication, as well as to send metadata back to the
        // client.
        grpc::ServerContext ctx_;

        // Let's implement a tiny state machine with the following states.
        enum CallStatus {
            CREATE, PROCESS, FINISH
        };
        CallStatus status_;  // The current serving state.

    private:
        virtual void CreateStatusFunc() = 0;

        virtual void ProcessStatusFunc() = 0;

        virtual void FinishStatusFunc() = 0;
    };

    template<class TRequest, class TReply>
    class CallData final : public BaseCallData {
    public:
        // Take in the "service" instance (in this case representing an asynchronous
        // server) and the completion queue "cq" used for asynchronous communication
        // with the gRPC runtime.
        CallData(paxos::Paxosme::AsyncService *service, grpc::ServerCompletionQueue *cq) : BaseCallData(service, cq),
                                                                                           responder_(&ctx_) {
            // Invoke the serving logic right away.
            Proceed();
        }


    protected:
        void InitRequestState() override;

        void Handle() override;

    private:

        // What we get from the client.
        TRequest request_;
        // What we send back to the client.
        TReply reply_;

        // The means to get back to the client.
        grpc::ServerAsyncResponseWriter<TReply> responder_;

        void CreateStatusFunc() override {
            // Make this instance progress to the PROCESS state.
            status_ = PROCESS;

            // As part of the initial CREATE state, we *request* that the system
            // start processing SayHello requests. In this request, "this" acts are
            // the tag uniquely identifying the request (so that different CallData
            // instances can serve different requests concurrently), in this case
            // the memory address of this CallData instance.
//                service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_,
//                                          this);
            InitRequestState();
        }

        void ProcessStatusFunc() override {
            // Spawn a new CallData instance to serve new clients while we process
            // the one for this CallData. The instance will deallocate itself as
            // part of its FINISH state.
            new CallData<TRequest, TReply>(service_, cq_);

            // The actual processing.
//                std::string prefix("Hello ");
//                reply_.set_message( prefix + request_.name());

            Handle();

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            status_ = FINISH;
            responder_.Finish(reply_, grpc::Status::OK, this);
        }


        void FinishStatusFunc() override {
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves (CallData).
            delete this;
        }

    };
}
#endif //PAXOSME_SERVER_HPP
