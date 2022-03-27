//
// Created by shiwk on 2021/3/11.
//

#include <server.hpp>

namespace paxosme {
    PaxosmeServer::PaxosmeServer(std::string &listening) {
        const std::string& server_address(listening);
        PaxosmeImpl paxosmeImpl;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&paxosmeImpl);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        server->Wait();
    }

    grpc::Status
    PaxosmeImpl::Propose(grpc::ServerContext *context, const paxos::ProposeRequest *request,
                         paxos::ProposeReply *response) {
        return Service::Propose(context, request, response);
    }

    grpc::Status
    PaxosmeImpl::Accept(grpc::ServerContext *context, const paxos::AcceptRequest *request,
                         paxos::AcceptReply *response) {
        return Service::Accept(context, request, response);
    }
}