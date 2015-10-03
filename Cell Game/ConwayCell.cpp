#include "ConwayCell.h"

namespace CellGame
{

void ConwayCell::init()
{
    alive = rand() % 100 < 50;
}

void ConwayCell::update()
{
    wasAlive = alive;
}

void ConwayCell::advance(std::array<AutomataCell*, DIRECTION_COUNT> neighbors)
{
    unsigned aliveNeighbors = 0;

    for (auto* neighbor : neighbors)
    {
        if (neighbor == nullptr) continue;

        auto* conway = static_cast<ConwayCell*>(neighbor);

        if (conway->wasAlive) ++aliveNeighbors;
    }

    alive = aliveNeighbors == 3 || aliveNeighbors == 2 && alive;
}

sf::Color ConwayCell::getColor()
{
    return alive ? sf::Color(255, 255, 255) : sf::Color(0, 0, 0);
}

}
