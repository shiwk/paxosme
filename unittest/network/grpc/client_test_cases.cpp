#include "client_test_helper.hpp"
#include "client.hpp"
#include <glog/logging.h>

TEST_F(GrpcClientTest, TestNewClient)
{
    auto client = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    EXPECT_TRUE(client != nullptr);
}

TEST_F(GrpcClientTest, TestSendOne)
{
    auto client = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    paxosme::PodMsg msg{paxosme::kPOD_MSG_PROPOSE_ACK};
    bool sendRes = client->Send(msg);
    ASSERT_TRUE(sendRes);
    ASSERT_EQ(1, increment_id_);
}

TEST_F(GrpcClientTest, TestSendMulti)
{
    auto client = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    int i = 1;
    for (; i < 10000; ++i)
    {
        paxosme::PodMsg msg{paxosme::kPOD_MSG_PROPOSE_ACK};
        bool sendRes = client->Send(msg);
        ASSERT_TRUE(sendRes);
        ASSERT_EQ(i, increment_id_);
    }
}

TEST_F(GrpcClientTest, TestMultiClients)
{
    auto client1 = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    auto client2 = std::shared_ptr<paxosme::GrpcClient>(paxosme::GrpcClient::NewClient(server.ToString()));
    int i = 1;
    for (; i < 10000; ++i)
    {
        paxosme::PodMsg msg{paxosme::kPOD_MSG_PROPOSE_ACK};
        auto client = i % 2 == 0 ? client1->shared_from_this() : client2->shared_from_this();
        bool sendRes = client->Send(msg);
        ASSERT_TRUE(sendRes);
        ASSERT_EQ(i, increment_id_);
    }
}