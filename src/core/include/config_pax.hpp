//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_CONFIG_PAX_HPP
#define PAXOSME_CONFIG_PAX_HPP
#include <cmath>
namespace paxosme {

    struct PaxConfig {
    private:
        node_id_t node_id_;
        int32_t node_count_;
        double majority_threshold_;
    public:
        double GetMajorityThreshold() const {
            return majority_threshold_;
        }

        void SetMajorityThreshold(double majority_threshold) {
            majority_threshold_ = majority_threshold;
        }

    public:
        int32_t GetNodeCount() const {
            return node_count_;
        }

        void SetNodeCount(int32_t node_count) {
            node_count_ = node_count;
        }

        int32_t GetMajorityCount() const {
            return ceil(majority_threshold_ * node_count_);
        }
    };
}


#endif //PAXOSME_CONFIG_PAX_HPP
