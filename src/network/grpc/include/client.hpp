//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_CLIENT_HPP
#define PAXOSME_CLIENT_HPP

#include "paxosme.grpc.pb.h"
using grpc::Channel;
using paxos::Paxosme;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using ::grpc::ChannelInterface;

class PaxosmeClient{
public:
    explicit PaxosmeClient( const std::shared_ptr<ChannelInterface>& channel)
    : stub_(Paxosme::NewStub(channel)) {}

    paxos::PrepareReply Prepare(paxos::PrepareRequest);

    paxos::ProposeReply Propose(paxos::ProposeRequest);
private:
    std::unique_ptr<Paxosme::Stub> stub_;
};
#endif //PAXOSME_CLIENT_HPP
