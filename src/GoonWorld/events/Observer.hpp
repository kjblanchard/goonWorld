#pragma once
#include <functional>
namespace GoonWorld
{
    class Event;
    struct Observer
    {
        using EventFunction = std::function<void(Event& event)>;
        inline Observer(int eventType, EventFunction func) : Function(func), EventType(eventType) {}
        ~Observer();
        EventFunction Function;
        int EventType;
    };
}