//
// Created by shiwk on 2021/3/5.
//

#ifndef PAXOSME_PAX_STORAGE_HPP
#define PAXOSME_PAX_STORAGE_HPP

#include "common.hpp"
#include <string>
#include <database.hpp>
#include <messages_pax.hpp>

namespace paxosme{
    class PaxStorage {
    public:
        int Write(instance_id_t instance_id, const PaxMessage & value);

        int Read(instance_id_t instance_id, const PaxMessage & value);

    private:
        Database * db;
    };
}


#endif //PAXOSME_PAX_STORAGE_HPP
