#pragma once
#include <GoonWorld/base/Component.hpp>
#include <GoonEngine/point.h>
#include <GoonPhysics/body.h>

struct SDL_Rect;
struct gpBody;
namespace GoonWorld
{
    class RigidbodyComponent : public Component
    {
    public:
        RigidbodyComponent(SDL_Rect *rect);
        static void PhysicsUpdate();
        inline gpVec& Acceleration() { return _body->acceleration; }

    private:
        static std::vector<RigidbodyComponent *> _currentRigidbodies;
        int _bodyNum;
        gpBody *_body;
    };
}