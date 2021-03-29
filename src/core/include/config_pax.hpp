//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_CONFIG_PAX_HPP
#define PAXOSME_CONFIG_PAX_HPP

namespace paxosme {

    class PaxConfig {
    private:
        double majority_threshold_;
        node_id_t node_id_;
        int32_t node_count_;
        bool only_learner_;
        node_id_t following_node_id_;
    };
}


#endif //PAXOSME_CONFIG_PAX_HPP
