//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include <communicator.hpp>

struct NetworkConfig{
    std::string listening;
};

class Network{
public:
    Network(Communicator *communicator): communicator_(communicator) {};
    void start();

private:
    Communicator* communicator_;
};

void Network::start() {

}
#endif //PAXOSME_NETWORK_HPP
