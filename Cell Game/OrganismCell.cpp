#include "OrganismCell.h"

namespace CellGame
{

void OrganismCell::init()
{
    alive = false;
}

void OrganismCell::update()
{
    prevAlive = alive;
}

void OrganismCell::advance(std::array<AutomataCell*, DIRECTION_COUNT> neighbors)
{
    unsigned aliveNeighbors = 0;

    for (auto* neighbor : neighbors)
    {
        if (neighbor == nullptr) continue;

        auto* organism = static_cast<OrganismCell*>(neighbor);

        if (organism->prevAlive) ++aliveNeighbors;
    }

    nextAlive = aliveNeighbors == 3 || aliveNeighbors == 2 && alive;
}

sf::Color OrganismCell::getColor()
{
    return alive ? sf::Color(255, 255, 255) : sf::Color(0, 0, 0);
}

}
