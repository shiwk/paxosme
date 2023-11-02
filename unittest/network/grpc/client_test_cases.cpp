#include "client_test_helper.hpp"
#include "client.hpp"
#include <glog/logging.h>

TEST_F(GrpcClientTest, TestNewClient)
{
    auto client = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    EXPECT_TRUE(client != nullptr);
}

TEST_F(GrpcClientTest, TestSend)
{
    auto client = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    paxosme::PodMsg msg{paxosme::kPOD_MSG_PROPOSE_ACK};
    client->Send(msg);
    LOG(INFO) << "TestSend FINISHED";
}