//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_SERVER_HPP
#define PAXOSME_SERVER_HPP

#include "paxosme.grpc.pb.h"
#include <grpcpp/server_builder.h>
#include <grpcpp/server.h>

using paxos::Paxosme;
using grpc::ServerBuilder;
using grpc::Server;
class NetworkConfig;

class PaxosmeImpl final : public Paxosme::Service{
public:

    grpc::Status Prepare(grpc::ServerContext* context, const paxos::PrepareRequest* request, paxos::PrepareReply* response) override ;

    grpc::Status Propose(grpc::ServerContext* context, const paxos::ProposeRequest* request, paxos::ProposeReply* response) override ;
private:

};

class PaxosmeServer {
    PaxosmeServer(NetworkConfig *network_config);
};
#endif //PAXOSME_SERVER_HPP
