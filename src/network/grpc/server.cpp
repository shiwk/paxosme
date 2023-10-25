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
        ServerBuilder builder;
        const std::string &serverAddress(endpoint.ToString());

        LOG(INFO) << "Server listening on " << serverAddress;
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&asyncService_);

        cq_ = std::move(builder.AddCompletionQueue());
        server_ = std::move(builder.BuildAndStart());

        msgCallback_ = msg_callback;

        // Proceed to the server's main loop.
        auto handleFunc = [this](){ return this->HandleRpcs(); };
        
        LOG(INFO) << "Start: handleFunc";
        handleLoop_ = std::move(std::async(std::launch::async, handleFunc));
        // HandleRpcs();
    }

    void GrpcServer::Shutdown()
    {
        if (is_shutdown_)
        {
            return;
        }
        server_->Shutdown();
        cq_->Shutdown();
    }

    // refer to https://github.com/grpc/grpc/blob/v1.52.0/examples/cpp/helloworld/greeter_async_server.cc
    void GrpcServer::HandleRpcs()
    {
        // Spawn a new CallData instance to serve new clients.
        auto shPtr1 = std::shared_ptr<CallData<paxos::ProposeRequest, paxos::ProposeReply>>(new CallData<paxos::ProposeRequest, paxos::ProposeReply>(&asyncService_, cq_.get(), msgCallback_));
        auto shPtr2 = std::shared_ptr<CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>>(new CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>(&asyncService_, cq_.get(), msgCallback_));
        auto shPtr3 = std::shared_ptr<CallData<paxos::AcceptRequest, paxos::AcceptReply>> (new CallData<paxos::AcceptRequest, paxos::AcceptReply>(&asyncService_, cq_.get(), msgCallback_));
        auto shPtr4 = std::shared_ptr <CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>>(new CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>(&asyncService_, cq_.get(), msgCallback_));
        auto shPtr5 = std::shared_ptr<CallData<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>>(new CallData<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>(&asyncService_, cq_.get(), msgCallback_));
        // todo I: check more call data
        void *tag; // uniquely identifies a request.
        bool ok;
        LOG(INFO) << "HandleRpcs";
#ifndef FINITE_SCHEDULE_LOOP
    while (cq_->Next(&tag, &ok))
        {
            LOG(INFO)<< "HandleRpcs: infinite loop";
            if (ok)
            {
                static_cast<BaseCallData *>(tag)->Proceed();
            }
        }
#endif
        
    }

    NetworkServer *NetworkServer::New()
    {
        return new GrpcServer;
    }
}