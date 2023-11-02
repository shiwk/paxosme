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

    void GrpcServer::Start(const Peer &endpoint, Network::MsgCallback msg_callback)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        ServerBuilder builder;
        const std::string &serverAddress(endpoint.ToString());

        LOG(INFO) << "Server listening on " << serverAddress;
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&asyncService_);

        cq_ = std::move(builder.AddCompletionQueue());
        server_ = std::move(builder.BuildAndStart());

        msgCallback_ = msg_callback;

        // Proceed to the server's main loop.
        auto handleFunc = [this]()
        { return this->HandleRpcs(); };

        LOG(INFO) << "Start: handleFunc";
        handleLoop_ = std::move(std::async(std::launch::async, handleFunc));
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
        // Spawn a new CallData instance to serve new clients.
        new CallData<paxos::ProposeRequest, paxos::ProposeReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::AcceptRequest, paxos::AcceptReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>(&asyncService_, cq_.get(), msgCallback_);
        // todo I: check more call data
        void *tag; // uniquely identifies a request.
        bool ok;
        LOG(INFO) << "Server handling..";
        while (cq_->Next(&tag, &ok))
        {
            if (ok)
            {
                static_cast<BaseCallData *>(tag)->Proceed();
            }
        }
    }

    NetworkServer *NetworkServer::New()
    {
        return new GrpcServer;
    }
}