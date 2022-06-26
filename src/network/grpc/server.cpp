//
// Created by shiwk on 2021/3/11.
//

#include <server.hpp>

namespace paxosme {
    GrpcServer::~GrpcServer() {
        server_->Shutdown();

    }

    void GrpcServer::Start(const Endpoint &endpoint, MsgCallback msg_callback) {
        const std::string &serverAddress(endpoint.ToString());
        ServerBuilder builder;

        //no authentication mechanism
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&asyncService_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();

        // Proceed to the server's main loop.
        HandleRpcs();

        msgCallback_ = msg_callback;
    }

    void GrpcServer::HandleRpcs() {
        // Spawn a new CallData instance to serve new clients.
        new CallData<paxos::ProposeRequest, paxos::ProposeReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>(&asyncService_, cq_.get(), msgCallback_);
        new CallData<paxos::AcceptRequest, paxos::AcceptReply>(&asyncService_, cq_.get(), msgCallback_);
        // todo I: check more call data
        void *tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a CallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or cq_ is shutting down.
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<BaseCallData *>(tag)->Proceed();
        }
    }
}