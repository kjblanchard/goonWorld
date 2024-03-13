#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonPhysics/body.h>
#include <GoonPhysics/scene.h>
#include <GoonWorld/base/GameObject.hpp>
using namespace GoonWorld;

std::vector<RigidbodyComponent *> RigidbodyComponent::_currentRigidbodies;

RigidbodyComponent::RigidbodyComponent(geRectangle *rect)
    : Component((int)ComponentTypes::Rigidbody), _static(false)
{
    auto bb = gpBBNew(rect->x, rect->y, rect->w, rect->h);
    _body = gpBodyNew(bb);
    _body->gravityEnabled = 1;
    _currentRigidbodies.push_back(this);
}
RigidbodyComponent::~RigidbodyComponent()
{
    gpSceneRemoveBody(_bodyNum);
    // gpBodyFree(_body);
}

bool RigidbodyComponent::IsOnGround()
{
    if (Game::GetTicks() == _isOnGroundCached)
        return _isOnGround;
    _isOnGroundCached = Game::GetTicks();
    return _isOnGround = gpBodyIsOnGround(_body);
    // return Game::GetTicks() == _isOnGroundCached ? _isOnGround : _isOnGround = gpBodyIsOnGround(_body);
}

void RigidbodyComponent::OnComponentAdd(GameObject &parent)
{
    _body->funcArgs = (void *)&parent;
    Component::OnComponentAdd(parent);
    if (_static)
    {
        _body->staticBody = true;
        _bodyNum = gpSceneAddStaticBody(_body);
    }
    else
    {
        _bodyNum = gpSceneAddBody(_body);
    }
}
void RigidbodyComponent::PhysicsUpdate()
{
    for (auto rigidbody : _currentRigidbodies)
    {
        if (!rigidbody->Parent())
            continue;

        rigidbody->Parent()->Location().x = rigidbody->_body->boundingBox.x;
        rigidbody->Parent()->Location().y = rigidbody->_body->boundingBox.y;
    }
}