//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_CONFIG_HPP
#define PAXOSME_CONFIG_HPP

#include <cmath>
#include <vector>
#include <set>
#include "common.hpp"

namespace paxosme {

    class ConfigInfo {
    private:
        node_id_t node_id_;
        double majority_threshold_;
        size_t node_count_;
        std::set<node_id_t> members; // todo II: members (and some key variables) to be persisted

    public:
        double GetMajorityThreshold() const {
            return majority_threshold_;
        }

        void SetMajorityThreshold(double majority_threshold) {
            majority_threshold_ = majority_threshold;
        }

        node_id_t GetNodeId() const {
            return node_id_;
        }

        size_t GetNodeCount() const {
            return node_count_;
        }

        // todo II: members (and some key variables) to be persisted
        size_t GetMajorityCount() const {
            return ceil(majority_threshold_ * (double)GetNodeCount());
        }

        bool ProposingAuthority() {
            return members.find(node_id_) != members.end();
        }
    };
}


#endif //PAXOSME_CONFIG_HPP
