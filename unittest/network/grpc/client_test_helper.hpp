#ifndef CLIENT_TEST_HELPER_HPP
#define CLIENT_TEST_HELPER_HPP

#include "server.hpp"
#include <gtest/gtest.h>
#include <glog/logging.h>

const std::string host = "127.0.0.1";
const int port = 9999;
const paxosme::Peer server{host, port};

class GrpcClientTest : public ::testing::Test
{
    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<paxosme::NetworkServer> server_;
};

void GrpcClientTest::SetUp()
{
    server_ = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New());
    paxosme::Network::MsgCallback msgCallBack = [](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
    };
    server_->Start(server, msgCallBack);
}

void GrpcClientTest::TearDown()
{
    server_->Shutdown();
}

#endif