#include "network_test_helper.hpp"
#include "network.hpp"
#include <glog/logging.h>

TEST_F(NetworkTests, TestInit)
{
    paxosme::EndPoint endpoint{"127.0.0.1", 9999};
    std::shared_ptr<paxosme::Network> network = ShortLife::CreateInstance<paxosme::Network>(endpoint);
    std::vector<paxosme::Peer> peers;
    peers.emplace_back(paxosme::Network::NodeIdToPeer(1));

    paxosme::Network::MsgCallback msgCallBack = [](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
    };
    auto networkOption = paxosme::Network::NetworkOptions{
        peers,
        1,
        msgCallBack};
    
    network->Start(networkOption);
}

TEST_F(NetworkTests, TestQuit)
{
    //todo
}