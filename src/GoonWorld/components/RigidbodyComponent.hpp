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
        inline bool IsOnGround() { return gpBodyIsOnGround(_body); }
        inline gpVec &Velocity() { return _body->velocity; }
        inline gpVec &Acceleration() { return _body->acceleration; }
        inline gpVec &MaxVelocity() { return _body->maxVelocity; }
        inline gpVec &Friction() { return _body->friction; }
        inline void SetBodyType(int bodyType) { _body->bodyType = bodyType; }
        // TODO move this
        gpBody *_body;

    private:
        void OnComponentAdd(GameObject &parent) override;
        static std::vector<RigidbodyComponent *> _currentRigidbodies;
        int _bodyNum;
    };
}