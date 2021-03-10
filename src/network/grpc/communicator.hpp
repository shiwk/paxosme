//
// Created by shiwk on 2021/3/6.
//

#ifndef PAXOSME_COMUNICATOR_HPP
#define PAXOSME_COMUNICATOR_HPP

//#include <network.hpp>
#include "communicate_pax.hpp"
#include "client.hpp"

class NetworkConfig;

class Communicator : public paxosme::PaxCommunicate {
public:
    explicit Communicator(NetworkConfig *network_config) : config_(network_config) {}

    int Send(node_id_t node_id, const paxosme::PaxMessage &pax_message) override;

    int Receive(const paxosme::PaxMessage &pax_message) override;

private:
    std::unordered_map<node_id_t, PaxosmeClient*> clientTable_;
    NetworkConfig* config_;
};

int Communicator::Send(node_id_t node_id, const paxosme::PaxMessage &pax_message) {
    return 0;
}

int Communicator::Receive(const paxosme::PaxMessage &pax_message) {
    return 0;
}

#endif //PAXOSME_COMUNICATOR_HPP
