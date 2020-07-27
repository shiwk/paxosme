//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_PAX_CONFIG_HPP
#define PAXOSME_PAX_CONFIG_HPP

namespace paxosme {

    class PaxConfig {
        double GetMajorityThreshold();

    private:
        double majority_threshold_;
    };
}


#endif //PAXOSME_PAX_CONFIG_HPP
