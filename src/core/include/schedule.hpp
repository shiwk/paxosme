//
// Created by shiwk on 2021/3/30.
//

#ifndef PAXOSME_SCHEDULE_HPP
#define PAXOSME_SCHEDULE_HPP

#include <event_queue.hpp>
#include <queue>
#include <unordered_map>

namespace paxosme {

    // for enum with std::hash & std::unordered_map
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    template <typename Key>
    using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

    template <typename Key, typename T>
    using MyUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;


    class Schedule {
    public:
//        void AddEvent(const EventHandler &cb, const time_t &when, EventType event_type);
//        void AddEvent(const EventHandler &cb, const timeval &when, EventType event_type);
        void AddEvent(const EventHandler &cb, const EventTimeStamp &when, EventType event_type);

        bool Dispatch(Event &);

        bool NextEventTime(EventTimeStamp &);

        void Remove(EventType);

        static Schedule* OneInstance() {
            static Schedule schedule;
            return &schedule;
        }

    private:
        MyUnorderedMap<EventType, EventId> eventTypeIdMap_;
        EventQueue eventQueue_;
        EventId eventId_;
    };
}

#endif //PAXOSME_SCHEDULE_HPP
