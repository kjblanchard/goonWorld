#pragma once
#include <GoonWorld/base/Component.hpp>
#include <GoonEngine/point.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/BodyTypes.hpp>
#include <GoonEngine/rectangle.h>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>

namespace GoonWorld
{
    class RigidbodyComponent : public Component
    {
    public:
        RigidbodyComponent(geRectangle *rect);
        ~RigidbodyComponent();
        static void PhysicsUpdate();
        bool IsOnGround();
        void AddOverlapFunction(int overlapType, OverlapFunc func);
        inline void GravityEnabled(bool isEnabled) { _body->gravityEnabled = isEnabled; }
        inline void SizeChange(Point newSize)
        {
            _body->boundingBox.w = newSize.x;
            _body->boundingBox.h = newSize.y;
        }
        inline gpVec &Velocity() { return _body->velocity; }
        inline gpVec &Acceleration() { return _body->acceleration; }
        inline gpVec &MaxVelocity() { return _body->maxVelocity; }
        inline gpVec &Friction() { return _body->friction; }
        inline void SetStaticBody(bool isStatic) { _static = isStatic; }
        inline void SetCollidesWithStaticBody(bool doesCollide) { _body->staticCollisionEnabled = doesCollide; }
        inline void SetBodyType(BodyTypes bodyType) { _body->bodyType = (int)bodyType; }
        inline void SetBodyType(int bodyType) { _body->bodyType = bodyType; }
        inline gpBB &BoundingBox() { return _body->boundingBox; }
        static inline void ResetRigidBodyVector() { _currentRigidbodies.clear(); }

    private:
        void OnComponentAdd(GameObject &parent) override;
        gpBody *_body;
        static std::vector<RigidbodyComponent *> _currentRigidbodies;
        int _bodyNum;
        long long _isOnGroundCached;
        bool _isOnGround, _static;
    };
    // template <typename T>
    // void RigidbodyComponent::AddOverlapFunction(int overlapType, std::function<void(void* args, gpBody *body, gpBody* overlapBody, gpOverlap *overlap)> func)
    // {
    // auto instance = dynamic_cast<T *>(_parent);
    // bodyOverlapArgs args;
    // args.bodyType = _body->bodyType;
    // args.overlapBodyType = overlapType;
    // args.overlapFunc = [func](void *args, gpBody *body, gpBody *overlapBody, gpOverlap *overlap)
    // {
    //     func(instance, overlapBody, overlap);
    // };
    // gpBodyAddOverlapBeginFunc(_body, func);
    // }
}