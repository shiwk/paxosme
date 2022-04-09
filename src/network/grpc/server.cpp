//
// Created by shiwk on 2021/3/11.
//

#include <server.hpp>

namespace paxosme {
    ServerImpl::~ServerImpl() {
        server_->Shutdown();

    }

    void ServerImpl::Start(std::string &listening) {
        const std::string &serverAddress(listening);
        ServerBuilder builder;

        //no authentication mechanism
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&asyncService_);

        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();

        // Proceed to the server's main loop.
        HandleRpcs();
    }

    void ServerImpl::HandleRpcs() {
        // Spawn a new CallData instance to serve new clients.
        new CallData<paxos::ProposeRequest, paxos::ProposeReply>(&asyncService_, cq_.get());
        // todo I: more call data
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

    grpc::Status
    PaxosmeGrpcImpl::Propose(grpc::ServerContext *context, const paxos::ProposeRequest *request,
                             paxos::ProposeReply *response) {
        return Service::Propose(context, request, response);
    }

    grpc::Status
    PaxosmeGrpcImpl::Accept(grpc::ServerContext *context, const paxos::AcceptRequest *request,
                            paxos::AcceptReply *response) {
        return Service::Accept(context, request, response);
    }


    template<>
    void CallData<paxos::ProposeRequest, paxos::ProposeReply>::InitRequestState() {
        service_->RequestPropose(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    template<>
    void CallData<paxos::ProposeRequest, paxos::ProposeReply> :: Handle(){
        //todo I : implement
    }
}