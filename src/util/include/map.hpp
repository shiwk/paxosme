//
// Created by kk on 2023/2/5.
//

#ifndef PAXOSME_MAP_HPP
#define PAXOSME_MAP_HPP

#include <unordered_map>
#include <mutex>
template<class TKey, class TValue>
class SafeMap {
public:
    void Add(TKey key, const TValue &value){
        std::unique_lock<std::mutex> lock(mtx_);
        dict_.insert(key, value);
    }

    bool Get(TKey key, TValue &value){
        std::unique_lock<std::mutex> lock(mtx_);
        if (dict_.find(key)==dict_.end())
            return false;

        value = dict_[key];
        return true;
    }

    TValue& operator[](TKey key){
        std::unique_lock<std::mutex> lock(mtx_);
        return dict_[key];
    }

private:
    std::unordered_map<TKey, TValue> dict_;
    mutable std::mutex  mtx_;
};

#endif //PAXOSME_MAP_HPP
