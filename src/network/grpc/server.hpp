//
// Created by shiwk on 2021/3/10.
//

#ifndef PAXOSME_SERVER_HPP
#define PAXOSME_SERVER_HPP

#include "paxosme.grpc.pb.h"

using paxos::Paxosme;
class PaxosmeServer final : public Paxosme::Service{
    
};
#endif //PAXOSME_SERVER_HPP
