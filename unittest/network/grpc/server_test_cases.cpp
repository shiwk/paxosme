#include <gtest/gtest.h>
#include "server.hpp"
#include <glog/logging.h>
#include "server_test_helper.hpp"

TEST_F(GrpcServerTest, TestStart)
{
    auto server = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New());
    std::vector<paxosme::Peer> peers;
    peers.emplace_back(paxosme::Network::NodeIdToPeer(1));

    paxosme::Network::MsgCallback msgCallBack = [](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
    };

    server->Start(paxosme::Peer{"127.0.0.1", 9999}, msgCallBack);
};