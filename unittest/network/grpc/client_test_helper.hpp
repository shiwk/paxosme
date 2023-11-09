#ifndef CLIENT_TEST_HELPER_HPP
#define CLIENT_TEST_HELPER_HPP

#include "server.hpp"
#include <gtest/gtest.h>
#include <glog/logging.h>

const std::string host = "127.0.0.1";
const int port = 9999;
const paxosme::EndPoint server{host, port};

class GrpcClientTest : public ::testing::Test
{
    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<paxosme::NetworkServer> server_;

public:
    template <class Func, class... Args>
    static void func(Func f, Args... a);
    int increment_id_ = 0;
};

template <class Func, class... Args>
void GrpcClientTest::func(Func f, Args... a)
{
    f(a...);
}


void GrpcClientTest::SetUp()
{
    server_ = std::shared_ptr<paxosme::NetworkServer>(paxosme::NetworkServer::New(server));
    paxosme::Network::MsgCallback msgCallBack = [this](const std::string message)
    {
        func([this]() { increment_id_++; });
        // DLOG(INFO) << "msgCallBack: " << message;
    };
    server_->Start(msgCallBack);
}

void GrpcClientTest::TearDown()
{
    server_->Shutdown();
}

#endif