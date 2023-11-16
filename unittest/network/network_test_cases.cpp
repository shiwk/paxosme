#include "network_test_helper.hpp"
#include "network.hpp"
#include "messages.hpp"
#include <glog/logging.h>

TEST_F(NetworkTests, PeerToNodeId)
{
    paxosme::Peer peer{"127.0.0.1", 9999};
    uint64_t nodeId = paxosme::Network::PeerToNodeId(peer);
    paxosme::Peer parsePeer = paxosme::Network::NodeIdToPeer(nodeId);
    ASSERT_EQ(peer, parsePeer);
}

TEST_F(NetworkTests, TestLaunch)
{
    paxosme::Peer peer{"127.0.0.1", 9999};
    std::vector<paxosme::Peer> peers;
    peers.emplace_back(peer);
    std::shared_ptr<paxosme::Network> network = ShortLife::CreateInstance<paxosme::Network>(peers);

    paxosme::Network::MsgCallback msgCallBack = [this](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
        func([this]()
             { increment_id_++; });
    };

    auto networkOption = paxosme::Network::NetworkOptions{
        peers,
        paxosme::Network::PeerToNodeId(peer),
        msgCallBack};

    network->Start(networkOption);  
    network->Quit(paxosme::Network::PeerToNodeId(peer)); 
}


TEST_F(NetworkTests, TestSend)
{
    paxosme::Peer peer{"127.0.0.1", 9999};
    
    std::vector<paxosme::Peer> peers;
    peers.emplace_back(peer);
    std::shared_ptr<paxosme::Network> network = ShortLife::CreateInstance<paxosme::Network>(peers);

    paxosme::Network::MsgCallback msgCallBack = [this](std::string message)
    {
        LOG(INFO) << "msgCallBack: " << message;
        func([this]()
             { increment_id_++; });
    };

    auto networkOption = paxosme::Network::NetworkOptions{
        peers,
        paxosme::Network::PeerToNodeId(peer),
        msgCallBack};

    network->Start(networkOption);

    paxosme::PaxMessage paxMessage;
    network->Send(peer, paxMessage);
    ASSERT_EQ(increment_id_, 1);
    // todo I: case go on
}