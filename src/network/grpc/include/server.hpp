//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_SERVER_HPP
#define PAXOSME_SERVER_HPP

#include "paxosme.grpc.pb.h"
#include "network_server.hpp"
#include <grpcpp/server_builder.h>
#include <grpcpp/server.h>
#include <future>
#include <utility>
#include <mutex>

using paxos::Paxosme;
using grpc::ServerBuilder;
using grpc::Server;

namespace paxosme {
    class GrpcServer : public NetworkServer {

    public:
        void Start(const Peer &, Network::MsgCallback) override;
        void Shutdown() override;
        ~GrpcServer() override;

    private:
        std::unique_ptr<Server> server_;
        paxos::Paxosme::AsyncService asyncService_;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        std::future<void> handleLoop_;
        bool is_running_ = false;
        void HandleRpcs();
        std::mutex mtx_;
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
        CallData(paxos::Paxosme::AsyncService *service, grpc::ServerCompletionQueue *cq, Network::MsgCallback msg_callback)
                : BaseCallData(service, cq), responder_(&ctx_), msgCallback_(std::move(msg_callback)) {
            // Invoke the serving logic right away.
            Proceed();
        }


    private:
        void InitRequestState();

        void HandleRequest(TReply &reply){
            msgCallback_(request_.SerializeAsString());
            reply.set_ack(true);
        }

        TRequest request_;
        grpc::ServerAsyncResponseWriter<TReply> responder_;

        Network::MsgCallback msgCallback_;

        void CreateStatusFunc() override {
            // Make this instance progress to the PROCESS state.
            status_ = PROCESS;

            InitRequestState();
        }

        void ProcessStatusFunc() override {
            // Spawn a new CallData instance to serve new clients while we process
            // the one for this CallData. The instance will deallocate itself as
            // part of its FINISH state.
            new CallData<TRequest, TReply>(service_, cq_, msgCallback_);


            TReply reply;
            HandleRequest(reply);

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            status_ = FINISH;
            responder_.Finish(reply, grpc::Status::OK, this);
        }


        void FinishStatusFunc() override {
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves (CallData).
            delete this;
        }

    };
}
#endif //PAXOSME_SERVER_HPP
