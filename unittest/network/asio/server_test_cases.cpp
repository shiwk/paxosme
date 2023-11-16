#include <gtest/gtest.h>
#include "udp_server.hpp"
#include <glog/logging.h>
#include "server_test_helper.hpp"

TEST_F(AsioServerTest, TestStart)
{
    auto server = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New(paxosme::Peer{"127.0.0.1", 9999}));

    paxosme::Network::MsgCallback msgCallBack = [](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
    };

    bool isRunning = server->Running();
    EXPECT_FALSE(isRunning);
    LOG(INFO) << "server->Running(): " << server->Running();


    server->Start(msgCallBack);
    isRunning = server->Running();
    EXPECT_TRUE(isRunning);
    LOG(INFO) << "server->Running(): " << server->Running();
    server.reset();
};

TEST_F(AsioServerTest, TestShuwdown)
{
    auto server = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New(paxosme::Peer{"127.0.0.1", 9999}));
    std::vector<paxosme::Peer> peers;
    peers.emplace_back(paxosme::Network::NodeIdToPeer(1));

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