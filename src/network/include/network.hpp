//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include "communicate_pax.hpp"
#include <vector>

namespace paxosme {
    struct NetworkConfig {
        std::string listening;
        std::vector<std::string> peers;
    };

    class Network {
    public:
        void Init();
        PaxCommunicator* GetCommunicator();

    private:
        PaxCommunicator *communicator_;
    };

}
#endif //PAXOSME_NETWORK_HPP
