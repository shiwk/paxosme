//
// Created by shiwk on 2021/3/11.
//

#include <server.hpp>

namespace paxosme {
    GrpcServer::~GrpcServer() {
        server_->Shutdown();
        cq_->Shutdown();
    }

    void GrpcServer::Start(const Peer &endpoint, Network::MsgCallback msg_callback) {
        const std::string &serverAddress(endpoint.ToString());
        ServerBuilder builder;

        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&asyncService_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();

        msgCallback_ = msg_callback;

        // Proceed to the server's main loop.
        HandleRpcs();
    }

    // refer to https://github.com/grpc/grpc/blob/v1.52.0/examples/cpp/helloworld/greeter_async_server.cc
    void GrpcServer::HandleRpcs() {
        // Spawn a new CallData instance to serve new clients.
        new CallData<paxos::ProposeRequest, paxos::ProposeReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::AcceptRequest, paxos::AcceptReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>(&asyncService_, cq_.get(), msgCallback_);
        // todo I: check more call data
        void *tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<BaseCallData *>(tag)->Proceed();
        }
    }

    NetworkServer* NetworkServer::New() {
        return new GrpcServer;
    }
}