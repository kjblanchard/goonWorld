#pragma once
#include <memory>
#include <list>
#include <GoonWorld/tiled/TiledMap.hpp>
#include <GoonWorld/interfaces/IUpdate.hpp>
#include <GoonWorld/interfaces/IStart.hpp>
#include <GoonWorld/interfaces/IEnable.hpp>
#include <GoonWorld/common/TimeSpan.hpp>
#include <GoonEngine/point.h>

namespace GoonWorld
{
    class Game;
    class Sound;
    class Component;
    class Timer;
    class GameObject : public IUpdate, public IStart, public IEnable
    {
    public:
        GameObject();
        GameObject(geRectangle *rect);
        GameObject(TiledMap::TiledObject);
        virtual ~GameObject();
        virtual void Start() override;
        virtual void Update() override;
        inline unsigned int Id() { return _id; }
        inline Point &Location() { return _location; }
        static inline void ClearGameObjects() { _gameobjects.clear(); }
        inline bool IsEnabled() const override { return _enabled; }
        virtual void Enabled(bool isEnabled) override;
        virtual void OnEnabled() override;
        virtual void OnDisabled() override;
        void AddComponent(Component *component);
        void AddComponent(std::initializer_list<Component *> componentList);
        Component *GetComponent(unsigned int componentType);
        template <typename T>
        T *GetComponentOfType(unsigned int componentType);
        static void UpdateTimers();
        static TimeSpan DeltaTime;

    protected:
        Sound &GetGameSound();
        Game &GetGame();
        void AddTimer(Timer *timer);
        inline void SetFlag(int &flags, int flag, bool onOff)
        {
            if (onOff)
            {
                flags |= flag;
            }
            else
            {
                flags &= ~flag;
            }
        }
        inline bool IsFlagSet(int &flags, int flag) { return (flags & flag); }
        // static std::vector<std::weak_ptr<GameObject>> _gameobjects;
        static std::vector<GameObject *> _gameobjects;
        unsigned int _id;
        Point _location;
        std::vector<std::shared_ptr<Component>> _components;

    private:
        static std::list<std::shared_ptr<Timer>> _timers;
        static unsigned int _numGameObjects;
        bool _enabled;
    };

    template <typename T>
    T *GameObject::GetComponentOfType(unsigned int componentType)
    {
        auto thing = GetComponent(componentType);
        return thing ? static_cast<T>(thing) : nullptr;
    }
}