#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/base/GameObject.hpp>
#include <GoonEngine/SdlWindow.h>
#include <GoonEngine/SdlSurface.h>
#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/core/Level.hpp>
using namespace GoonWorld;

DebugDrawComponent::DebugDrawComponent(Point size)
    : Component((int)ComponentTypes::DebugDrawComponent), DrawColor{255, 0, 0, 255}, Size(size), _visible(true)
{
}
void DebugDrawComponent::Draw()
{
    if (!_enabled)
        return;
    geRectangle dstRect{
        _parent->Location().x,
        _parent->Location().y,
        Size.x,
        Size.y};
    geDrawDebugRect(&dstRect, &DrawColor);
}
void DebugDrawComponent::OnComponentAdd(GameObject &parent)
{
    Component::OnComponentAdd(parent);
    Enabled(true);
    // GetGame().AddDrawObject(this);
    GetGame().GetCurrentLevel().AddDrawObject(this);
}
