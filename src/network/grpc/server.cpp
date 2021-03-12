//
// Created by shiwk on 2021/3/11.
//

#include <server.hpp>
#include <network.hpp>


PaxosmeServer::PaxosmeServer(NetworkConfig *network_config) {
    std::string server_address(network_config->listening);
    PaxosmeImpl paxosmeImpl;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&paxosmeImpl);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

grpc::Status
PaxosmeImpl::Prepare(grpc::ServerContext *context, const paxos::PrepareRequest *request, paxos::PrepareReply *response) {
    return Service::Prepare(context, request, response);
}

grpc::Status
PaxosmeImpl::Propose(grpc::ServerContext *context, const paxos::ProposeRequest *request, paxos::ProposeReply *response) {
    return Service::Propose(context, request, response);
}
