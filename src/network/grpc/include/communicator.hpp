//
// Created by shiwk on 2021/3/6.
//

#ifndef PAXOSME_COMUNICATOR_HPP
#define PAXOSME_COMUNICATOR_HPP

#include <communicate_pax.hpp>
#include <client.hpp>

namespace paxosme {
    class NetworkConfig;

    class Communicator : public PaxCommunicator {
    public:
        explicit Communicator(std::vector<node_id_t> &);

        int Send(node_id_t node_id, const paxosme::PaxMessage &pax_message) override;

        int Receive(const paxosme::PaxMessage &pax_message) override;

    private:
        std::string ParseNodeId(node_id_t);
        std::shared_ptr<GrpcClient> NewClient(std::string &);
        std::unordered_map<node_id_t, std::shared_ptr<GrpcClient>> clientTable_;
    };
}
#endif //PAXOSME_COMUNICATOR_HPP
