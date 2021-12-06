//
// Created by shiwk on 2020/7/18.
//


#include <proposer_pax.hpp>

bool paxosme::ProposalCounter::IsMajorityAccepted() {
    return approvals_.size() >= pax_config_->GetMajorityCount();
}

bool paxosme::ProposalCounter::IsMajorityPromised() {
    return promises_.size() >= pax_config_->GetMajorityCount();
}

bool paxosme::ProposalCounter::IsMajorityRejected() {
    return rejections_.size() >= pax_config_->GetMajorityCount();
}

bool paxosme::ProposalCounter::IsStillPending() {
    return vote_count_ >= pax_config_ -> GetNodeCount();
}

bool paxosme::ProposalCounter::AddAccept(proposal_id_t proposal_id, node_id_t node_id) {
    if (approvals_.find(node_id) != approvals_.end())
        return false;
    approvals_.insert(node_id);
    vote_count_++;
    return true;
}

bool paxosme::ProposalCounter::AddRejection(proposal_id_t proposal_id, node_id_t node_id) {
    if (rejections_.find(node_id) != rejections_.end())
        return false;
    rejections_.insert(node_id);
    vote_count_++;
    return true;
}

bool paxosme::ProposalCounter::AddPromise(proposal_id_t, node_id_t node_id) {
    if (promises_.find(node_id) != promises_.end())
        return false;
    promises_.insert(node_id);
    vote_count_++;
    return true;
}

void paxosme::ProposalCounter::Reset() {
    rejections_.clear();
    approvals_.clear();
    promises_.clear();
    vote_count_ = 0;
}

