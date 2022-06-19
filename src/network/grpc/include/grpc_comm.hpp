//
// Created by shiwk on 2021/3/6.
//

#ifndef PAXOSME_COMUNICATOR_HPP
#define PAXOSME_COMUNICATOR_HPP

#include <communicate_pax.hpp>
#include <client.hpp>
#include "network.hpp"

namespace paxosme {

class GrpcComm : public Communicator<PaxMessage> {
    public:
        explicit GrpcComm(std::vector<node_id_t> &);
    private:

        static std::string ParseNodeId(node_id_t);

        static std::shared_ptr<GrpcClient> NewClient(std::string &);

        std::unordered_map<node_id_t, std::shared_ptr<GrpcClient>> clientTable_;

        template<class TRequest, class TResponse>
        bool Send(node_id_t, TRequest &);

        template<class TRequest, class TResponse>
        void Broadcast(TRequest &);

        friend class Communicator<PaxMessage>;
    };

    template<class TRequest, class TResponse>
    void GrpcComm::Broadcast(TRequest &request) {
        for (const auto &kv: clientTable_) {
            node_id_t nodeId = kv.first;
            auto client = kv.second;
            TResponse response;
            client->AsyncCall(request, response);
        }
    }

    template<class TRequest, class TResponse>
    bool GrpcComm::Send(node_id_t node_id, TRequest &request) {
        if (clientTable_.find(node_id) == clientTable_.end())
            return false;

        auto client = clientTable_[node_id];
        TResponse response;
        return client->AsyncCall(request, response);
    }
}
#endif //PAXOSME_COMUNICATOR_HPP
