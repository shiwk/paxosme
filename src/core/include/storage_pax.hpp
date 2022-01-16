//
// Created by shiwk on 2021/3/5.
//

#ifndef PAXOSME_STORAGE_PAX_HPP
#define PAXOSME_STORAGE_PAX_HPP

#include "common.hpp"
#include <string>
#include <database.hpp>
#include <messages_pax.hpp>
#include <state.pb.h>

namespace paxosme{
    class Storage {
    public:
        int Write(instance_id_t instance_id, const PaxosState & value);

        int Read(instance_id_t instance_id, PaxosState & value);
    private:
        Database * db;
    };
}


#endif //PAXOSME_STORAGE_PAX_HPP
