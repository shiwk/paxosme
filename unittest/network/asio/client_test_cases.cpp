#include "client_test_helper.hpp"
#include "udp_client.hpp"

TEST_F(UdpClientTest, TestNewClient)
{
    auto client = std::shared_ptr<paxosme::NetworkClient>(paxosme::NetworkClient::New(paxosme::Peer{host, port}));
    ASSERT_TRUE(client != nullptr);
}

TEST_F(UdpClientTest, TestSendOne)
{
    auto client = std::shared_ptr<paxosme::NetworkClient>(paxosme::NetworkClient::New(paxosme::Peer{host, port}));
    ASSERT_TRUE(client != nullptr);
    bool sendRes = client->Send(paxosme::PodMsg{paxosme::kPOD_MSG_PROPOSE_ACK, "test"});
    ASSERT_TRUE(sendRes);
    ASSERT_EQ(1, increment_id_);
}

TEST_F(UdpClientTest, TestSendMulti)
{
    auto client = std::shared_ptr<paxosme::NetworkClient>(paxosme::UdpClient::NewClient(paxosme::Peer{host, port}));
    int i = 1;
    for (; i < 10000; ++i)
    {
        paxosme::PodMsg msg{paxosme::kPOD_MSG_PROPOSE_ACK};
        bool sendRes = client->Send(paxosme::PodMsg{paxosme::kPOD_MSG_PROPOSE_ACK, "test"});
        ASSERT_TRUE(sendRes);
        ASSERT_EQ(i, increment_id_);
    }
}

TEST_F(UdpClientTest, TestMultiClients)
{
    auto client1 = std::shared_ptr<paxosme::UdpClient>(paxosme::UdpClient::NewClient(paxosme::Peer{host, port}));
    auto client2 = std::shared_ptr<paxosme::UdpClient>(paxosme::UdpClient::NewClient(paxosme::Peer{host, port}));
    int i = 1;
    for (; i < 10000; ++i)
    {
        paxosme::PodMsg msg{paxosme::kPOD_MSG_PROPOSE_ACK};
        auto client = i % 2 == 0 ? client1->shared_from_this() : client2->shared_from_this();
        bool sendRes = client->Send(paxosme::PodMsg{paxosme::kPOD_MSG_PROPOSE_ACK, "test"});
        ASSERT_TRUE(sendRes);
        ASSERT_EQ(i, increment_id_);
    }
}