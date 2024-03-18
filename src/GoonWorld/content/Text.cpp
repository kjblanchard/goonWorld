#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/text.h>
#include <GoonEngine/SdlSurface.h>
using namespace std;
using namespace GoonWorld;

Text::Text(string stringToLoad, Point location, geColor textColor)
    : _stringToDisplay(stringToLoad), _textColor(textColor), _boundingBox(geRectangleZero()), _isVisible(true)
{
    _boundingBox.x = location.x;
    _boundingBox.y = location.y;
}

Text::~Text()
{
    printf("Text unloading\n");
    Unload();
}

void Text::Draw()
{
    if (!IsVisible())
        return;
    geRectangle testBox = _boundingBox;
    testBox.w /= 2;
    testBox.h /= 2;
    geDrawTexture(_loadedTexture, NULL, &_boundingBox, false);
    // geDrawTexture(_loadedTexture, NULL, &testBox, false);
}

void Text::Visible(bool isVisible)
{
    _isVisible = isVisible;
}

bool Text::IsVisible()
{
    return _isVisible;
}

void Text::Load()
{
    Point textureDimensions = gePointZero();
    _loadedTexture = geCreateTextureForString(_stringToDisplay.c_str(), &textureDimensions, _textColor);
    _boundingBox.w = textureDimensions.x;
    _boundingBox.h = textureDimensions.y;
    Game::Instance()->AddUIObject(this);
}

void Text::Unload()
{
    DestroyTexture(_loadedTexture);
}