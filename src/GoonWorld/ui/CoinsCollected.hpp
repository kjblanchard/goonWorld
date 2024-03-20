#pragma once
#include <vector>
#include <GoonWorld/interfaces/IDraw.hpp>

namespace GoonWorld
{
    class Text;
    class CoinsCollectedUI : public IDraw
    {
    public:
        CoinsCollectedUI();
        void UpdateCoins(int coins);
        int _currentCoins;
        int x;
        int y;
        Text *_coinText;
        int numberOffset;
        std::vector<Text *> _loadedNumbers;
        void Draw() override;
        void Visible(bool isVisible) override;
        bool IsVisible() override;
    };

}