#ifndef CLIENT_TEST_HELPER_HPP
#define CLIENT_TEST_HELPER_HPP

#include <gtest/gtest.h>
#include <glog/logging.h>
#include "udp_server.hpp"

const std::string host = "127.0.0.1";
const uint32_t port = 9999;

class UdpClientTest : public ::testing::Test
{
    void SetUp() override;
    void TearDown() override;
    std::shared_ptr<paxosme::NetworkServer> server_;

    template <class Func, class... Args>
    static void func(Func f, Args... a);

protected:
    int increment_id_ = 0;
};

template <class Func, class... Args>
void UdpClientTest::func(Func f, Args... a)
{
    f(a...);
}

void UdpClientTest::SetUp()
{
    server_ = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New(paxosme::Peer{host, port}));

    paxosme::Network::MsgCallback msgCallBack = [this](std::string message)
    {
        func([this]()
             { increment_id_++; });
        // DLOG(INFO) << "msgCallBack: " << message;
    };

    server_->Start(msgCallBack);
}

void UdpClientTest::TearDown()
{
    server_->Shutdown();
}

#endif