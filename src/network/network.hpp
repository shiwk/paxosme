//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include "grpc/communicator.hpp"

class NetworkConfig{

};

class Network{
public:
    Network(Communicator *pax_communicator): communicator_(pax_communicator) {};
    void start();

private:
    Communicator* communicator_;
};

void Network::start() {

}
#endif //PAXOSME_NETWORK_HPP
