#pragma once
#include <GoonWorld/base/Component.hpp>
#include <GoonEngine/point.h>
#include <GoonPhysics/body.h>
#include <GoonWorld/BodyTypes.hpp>
#include <GoonEngine/rectangle.h>
#include <GoonPhysics/body.h>
#include <GoonPhysics/overlap.h>
#include <GoonWorld/interfaces/IDraw.hpp>

namespace GoonWorld
{
    class BoxColliderComponent;
    class RigidbodyComponent : public Component, public IDraw
    {
    public:
        RigidbodyComponent(geRectangle *rect, Point offset = gePointZero());
        ~RigidbodyComponent();
        static void PhysicsUpdate();
        bool IsOnGround();
        void AddOverlapFunction(int overlapType, OverlapFunc func);
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
        inline bool JustGotOnGround(void) { return gpBodyJustGotOnGround(_body); }
        inline void SetOverlapsEnabled(bool enabled) { _body->enabled = enabled; }
        inline bool JustLeftGround(void) { return gpBodyJustNotOnGround(_body); }
        inline void GravityEnabled(bool isEnabled)
        {
            _isGravityEnabled = false;
            _body->gravityEnabled = isEnabled;
        }
        inline void XGravityEnabled(bool isEnabled) { _body->xGravityEnabled = isEnabled; }
        inline void YGravityEnabled(bool isEnabled) { _body->yGravityEnabled = isEnabled; }
        void OnEnabled() override;
        void OnDisabled() override;
        inline void SetLocation(Point location)
        {
            _body->boundingBox.x = location.x;
            _body->boundingBox.y = location.y;
        }
        inline void SizeChange(Point newSize)
        {
            _body->boundingBox.w = newSize.x;
            _body->boundingBox.h = newSize.y;
        }
        inline void SetDebug(bool debug) { _debugDraw = debug; }
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
        inline void AddBoxCollider(BoxColliderComponent *box) { _boxColliders.push_back(box); }

    private:
        void OnComponentAdd(GameObject &parent) override;
        static void OnBodyUpdate(void *args, gpBody *body);
        gpBody *_body;
        static std::vector<RigidbodyComponent *> _currentRigidbodies;
        int _bodyNum;
        bool _isOnGround = false, _static = false, _isGravityEnabled = true;
        Point _offset;
        bool _debugDraw = false;
        std::vector<BoxColliderComponent *> _boxColliders;
    };
}