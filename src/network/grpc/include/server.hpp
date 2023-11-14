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
#include <glog/logging.h>

using grpc::Server;
using grpc::ServerBuilder;
using paxos::Paxosme;

namespace paxosme
{
    class GrpcServer : public NetworkServer
    {

    public:
        GrpcServer(const EndPoint &);
        void Start(const Network::MsgCallback &) override;
        void Shutdown() override;
        bool Running() override;
        ~GrpcServer() override;

    private:
        std::unique_ptr<Server> server_;
        paxos::Paxosme::AsyncService asyncService_;
        std::shared_ptr<grpc::ServerCompletionQueue> cq_;
        std::future<void> handleLoop_;
        bool is_running_ = false;
        void HandleRpcs();
        void PrepareCallData();
        std::mutex mtx_;
        std::string serverAddress_;
    };

    class BaseCallData
    {
    public:
        // Let's implement a tiny state machine with the following states.
        enum CallStatus
        {
            DUMMY,
            INITIALIZED,
            SERVED,
            FINISH
        };

        void Proceed()
        {
            const std::string &typeName = typeid(*this).name();
            DLOG(INFO) << typeName << " Proceed: " << status_;
            if (status_ == DUMMY)
            {
                DLOG(INFO) << "Proceed: CreateStatusFunc(";
                CreateStatusFunc();
            }
            else if (status_ == INITIALIZED)
            {
                DLOG(INFO) << "Proceed: ProcessStatusFunc(";
                ProcessStatusFunc();
            }
            else if (status_ == SERVED)
            {
                DLOG(INFO) << "Proceed: FinishStatusFunc(";
                FinishStatusFunc();
            }
        }

        virtual bool Finished() const = 0;
        virtual ~BaseCallData() = default;

    protected:
        BaseCallData(paxos::Paxosme::AsyncService *service, std::shared_ptr<grpc::ServerCompletionQueue> cq) : service_(service),
                                                                                                               cq_(cq),
                                                                                                               status_(DUMMY) {}
        // The means of communication with the gRPC runtime for an asynchronous
        // server.
        paxos::Paxosme::AsyncService *service_;
        // The producer-consumer queue where for asynchronous server notifications.
        std::shared_ptr<grpc::ServerCompletionQueue> cq_;
        // Context for the rpc, allowing to tweak aspects of it such as the use
        // of compression, authentication, as well as to send metadata back to the
        // client.
        grpc::ServerContext ctx_;

        CallStatus status_; // The current serving state.

    private:
        virtual void CreateStatusFunc() = 0;

        virtual void ProcessStatusFunc() = 0;

        virtual void FinishStatusFunc() = 0;
    };

    class CallDataManager
    {
    public:
        static void Procceed(std::unique_ptr<BaseCallData> &&callData);

        template <class TRequest, class TReply>
        static void New(paxos::Paxosme::AsyncService *service, std::shared_ptr<grpc::ServerCompletionQueue> cq, Network::MsgCallback msg_callback);  
    };
    

    template <class TRequest, class TReply>
    class CallData final : public BaseCallData
    {
    public:
        CallData(paxos::Paxosme::AsyncService *service, std::shared_ptr<grpc::ServerCompletionQueue> cq, Network::MsgCallback msg_callback)
            : BaseCallData(service, cq), responder_(&ctx_), msgCallback_(std::move(msg_callback))
        {
            // Invoke the serving logic right away.
            Proceed();
        }

        ~CallData()
        {
            DLOG(INFO) << "CallData: ~CallData()";
        };

        bool Finished() const override
        {
            return status_ == FINISH;
        }

    private:
        void InitRequestState();

        void HandleRequest(TReply &reply)
        {
            msgCallback_(request_.SerializeAsString());
            reply.set_ack(true);
        }

        TRequest request_;
        grpc::ServerAsyncResponseWriter<TReply> responder_;

        Network::MsgCallback msgCallback_;

        void CreateStatusFunc() override
        {
            // Make this instance progress to the INITIALIZED state.
            InitRequestState();
            status_ = INITIALIZED;
        }

        void ProcessStatusFunc() override
        {
            // Spawn a new CallData instance to serve new clients while we process
            // the one for this CallData. The instance will deallocate itself as
            // part of its FINISH state.
            CallDataManager::New<TRequest, TReply>(service_, cq_, msgCallback_);

            TReply reply;
            HandleRequest(reply);

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            status_ = SERVED;
            responder_.Finish(reply, grpc::Status::OK, this);
        }

        void FinishStatusFunc() override
        {
            GPR_ASSERT(status_ == SERVED);
            status_ = FINISH;
            // Once in the FINISH state, deallocate ourselves (CallData).
            // delete this;
        }
    };
}
#endif // PAXOSME_SERVER_HPP
