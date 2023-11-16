#include <gtest/gtest.h>
#include "server.hpp"
#include <glog/logging.h>
#include "server_test_helper.hpp"

TEST_F(GrpcServerTest, TestStart)
{
    auto server = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New(paxosme::Peer{"127.0.0.1", 9999}));

    paxosme::Network::MsgCallback msgCallBack = [](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
    };

    bool isRunning = server->Running();
    EXPECT_FALSE(isRunning);

    server->Start(msgCallBack);
    isRunning = server->Running();
    EXPECT_TRUE(isRunning);
};

TEST_F(GrpcServerTest, TestShuwdown)
{
    auto server = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New(paxosme::Peer{"127.0.0.1", 9999}));

    paxosme::Network::MsgCallback msgCallBack = [](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
    };

    server->Start(msgCallBack);
    bool isRunning = server->Running();
    EXPECT_TRUE(isRunning);

    server->Shutdown();
    isRunning = server->Running();
    EXPECT_FALSE(isRunning);
};