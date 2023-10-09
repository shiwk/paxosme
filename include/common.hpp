//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_COMMON_H
#define PAXOSME_COMMON_H

#define NODE_DUMMY 0
#define PROPOSAL_DUMMY 0
#define INSTANCE_DUMMY 0

#include <cstdint>
#include <string>
#include <bitset>
#include <iomanip> // std::setw
#include <sstream>

using node_id_t = uint64_t;
using proposal_id_t = uint64_t;
using instance_id_t = uint64_t;
using index_t = uint64_t;
using LogValue = std::string;

class LongLife
{
public:
    template <class TClass>
    static TClass *Instance() { return TClass::SingleInstance(); }
};

class ShortLife
{
public:
    template <class TClass>
    static std::shared_ptr<TClass> CreateInstance() { return std::shared_ptr<TClass>(TClass::New()); }
};

class SizeString
{
public:
    template <class T>
    static std::string ToBitString(const T t)
    {
        std::bitset<BitStringSize<T>()> bs(t);
        return bs.to_string();
    }

    template <class T>
    static size_t BitStringSize()
    {
        return sizeof(T) * 8;
    }

    template <typename T>
    static std::string ToHexString(const T t)
    {
        std::stringstream stream;
        stream << std::setfill('0')
               << std::setw(HexStringSize<T>())
               << std::hex << t;
        return stream.str();
    }

    template <typename T>
    static size_t HexStringSize()
    {
        return sizeof(T) * 2;
    }
};

#endif // PAXOSME_COMMON_H
