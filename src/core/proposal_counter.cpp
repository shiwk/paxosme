//
// Created by shiwk on 2020/7/18.
//


#include <proposer_pax.hpp>
namespace paxosme {

    ProposalCounter::ProposalCounter(const PaxConfig * config): pax_config_(const_cast<PaxConfig *>(config)), vote_count_(0){}

    bool ProposalCounter::IsMajorityAccepted() {
        return approvals_.size() >= pax_config_->GetMajorityCount();
    }

    bool ProposalCounter::IsMajorityPromised() {
        return promises_.size() >= pax_config_->GetMajorityCount();
    }

    bool ProposalCounter::IsMajorityRejected() {
        return rejections_.size() >= pax_config_->GetMajorityCount();
    }

    bool ProposalCounter::IsStillPending() {
        return vote_count_ >= pax_config_->GetNodeCount();
    }

    bool ProposalCounter::AddAccept(proposal_id_t proposal_id, node_id_t node_id) {
        if (approvals_.find(node_id) != approvals_.end())
            return false;
        approvals_.insert(node_id);
        vote_count_++;
        return true;
    }

    bool ProposalCounter::AddRejection(proposal_id_t proposal_id, node_id_t node_id) {
        if (rejections_.find(node_id) != rejections_.end())
            return false;
        rejections_.insert(node_id);
        vote_count_++;
        return true;
    }

    bool ProposalCounter::AddPromise(proposal_id_t, node_id_t node_id) {
        if (promises_.find(node_id) != promises_.end())
            return false;
        promises_.insert(node_id);
        vote_count_++;
        return true;
    }

    void ProposalCounter::Reset() {
        rejections_.clear();
        approvals_.clear();
        promises_.clear();
        vote_count_ = 0;
    }
}