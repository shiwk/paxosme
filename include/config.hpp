//
// Created by shiwk on 2020/7/25.
//

#ifndef PAXOSME_CONFIG_HPP
#define PAXOSME_CONFIG_HPP

#include <cmath>
#include <vector>
#include <set>

namespace paxosme {

    struct Peer {
        node_id_t node_id;
        std::string ip;
        int port;
    };

    class PaxConfig {
    private:
        node_id_t node_id_;
        double majority_threshold_;
        std::vector<Peer> peers_;
        std::set<node_id_t> members;

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
            return peers_.size();
        }

        size_t GetMajorityCount() const {
            return ceil(majority_threshold_ * (double)GetNodeCount());
        }

        bool ProposingAuthority() {
            return members.find(node_id_) != members.end();
        }
    };
}


#endif //PAXOSME_CONFIG_HPP
