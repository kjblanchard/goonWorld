#include <GoonWorld/ui/CoinsCollected.hpp>
#include <GoonWorld/content/Text.hpp>
#include <GoonWorld/core/Content.hpp>
#include <GoonWorld/core/Game.hpp>
using namespace GoonWorld;

CoinsCollectedUI::CoinsCollectedUI()
    : _currentCoins(0), x(50), y(20)
{
    for (size_t i = 0; i < 10; i++)
    {
        auto text = new Text(std::to_string(i), gePointZero());
        Content::CreateContent(text);
        text->Visible(false);
        _loadedNumbers.push_back(text);
    }
    Game::Instance()->AddUIObject(this);
}

void CoinsCollectedUI::UpdateCoins(int coins)
{
    int tens = _currentCoins / 10;
    int ones = _currentCoins % 10;
    // Set old numbers to not visible
    _loadedNumbers[tens]->Visible(false);
    _loadedNumbers[ones]->Visible(false);
    _currentCoins = coins;
    // Set new numbers to visible
    tens = _currentCoins / 10;
    ones = _currentCoins % 10;
    _loadedNumbers[tens]->Visible(true);
    _loadedNumbers[ones]->Visible(true);
}

void CoinsCollectedUI::Draw()
{
    int tens = _currentCoins / 10;
    int ones = _currentCoins % 10;
    _loadedNumbers[tens]->SetLocation(Point{x, y});
    _loadedNumbers[tens]->Draw();
    _loadedNumbers[ones]->SetLocation(Point{x + _loadedNumbers[tens]->Width(), y});
    _loadedNumbers[ones]->Draw();
}

void CoinsCollectedUI::Visible(bool isVisible)
{
}

bool CoinsCollectedUI::IsVisible()
{
    return true;
}