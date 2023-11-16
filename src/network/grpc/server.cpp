//
// Created by shiwk on 2021/3/11.
//

#include <server.hpp>
#include <glog/logging.h>

namespace paxosme
{
    GrpcServer::~GrpcServer()
    {
        Shutdown();
    }

    GrpcServer::GrpcServer(const Peer &peer) : serverAddress_(peer.ToString())
    {
    }

    void GrpcServer::Start(Network::MsgCallback msg_callback)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        ServerBuilder builder;

        LOG(INFO) << "Server listening on " << serverAddress_;
        builder.AddListeningPort(serverAddress_, grpc::InsecureServerCredentials());

        builder.RegisterService(&asyncService_);

        cq_ = std::move(builder.AddCompletionQueue());
        server_ = std::move(builder.BuildAndStart());

        msgCallback_ = std::move(msg_callback);

        // Proceed to the server's main loop.
        handleLoop_ = std::move(std::async(std::launch::async, [this]()
                                           {
            LOG(INFO) << "Start: handleFunc";
            this->PrepareCallData();
            return this->HandleRpcs(); }));

        // HandleRpcs();
        is_running_ = true;
    }

    void GrpcServer::Shutdown()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        if (!is_running_)
        {
            return;
        }
        server_->Shutdown();
        cq_->Shutdown();
        LOG(INFO) << "Server shutdown";
        is_running_ = false;
    }

    bool GrpcServer::Running()
    {
        return is_running_;
    }

    // refer to https://github.com/grpc/grpc/blob/v1.52.0/examples/cpp/helloworld/greeter_async_server.cc
    void GrpcServer::HandleRpcs()
    {

        void *tag; // uniquely identifies a request.
        bool ok;
        LOG(INFO) << "Server handling..";
        while (cq_->Next(&tag, &ok))
        {
            if (ok)
            {
                auto callDataPtr = std::unique_ptr<BaseCallData>(static_cast<BaseCallData *>(tag));
                CallDataManager::Procceed(std::move(callDataPtr));
            }
        }
    }

    void GrpcServer::PrepareCallData()
    {
        // Spawn a new CallData instance to serve new clients.
        CallDataManager::New<paxos::ProposeRequest, paxos::ProposeReply>(&asyncService_, cq_, msgCallback_);
        CallDataManager::New<paxos::ProposeAckRequest, paxos::ProposeAckReply>(&asyncService_, cq_, msgCallback_);
        CallDataManager::New<paxos::AcceptRequest, paxos::AcceptReply>(&asyncService_, cq_, msgCallback_);
        CallDataManager::New<paxos::AcceptAckRequest, paxos::AcceptAckReply>(&asyncService_, cq_, msgCallback_);
        CallDataManager::New<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>(&asyncService_, cq_, msgCallback_);
        // todo I: check more call data
    }

    NetworkServer *NetworkServer::New(const Peer &peer)
    {
        return new GrpcServer(peer);
    }

    void CallDataManager::Procceed(std::unique_ptr<BaseCallData> &&callDataPtr)
    {
        callDataPtr->Proceed();
        if (callDataPtr->Finished())
        {
            return;
        }
        callDataPtr.release();
    }

    template <class TRequest, class TReply>
    void CallDataManager::New(paxos::Paxosme::AsyncService *service, std::shared_ptr<grpc::ServerCompletionQueue> cq, Network::MsgCallback msg_callback)
    {
        new CallData<TRequest, TReply>(service, cq, std::move(msg_callback));
    }
}