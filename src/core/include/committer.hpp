//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_COMMITTER_HPP
#define PAXOSME_COMMITTER_HPP

#include <log_value.hpp>

namespace paxosme {
    class Committer {
        void PreCommit(LogValue log_value);
    };
}
#endif //PAXOSME_COMMITTER_HPP
