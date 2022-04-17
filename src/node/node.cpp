//
// Created by shiwk on 2020/7/25.
//

#include "network.hpp"

int main(int argc, char **argv) {
    paxosme::Network network;
    std::vector<node_id_t> nodes;
    paxosme::NetworkConfig networkConfig{80, nodes};
    network.Init(networkConfig);
    auto communicator = network.GetCommunicator();

}
