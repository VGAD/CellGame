#include "OrganismCell.h"

namespace CellGame
{

void OrganismCell::init()
{
    alive = false;
}

void OrganismCell::update()
{
    wasAlive = alive;
}

void OrganismCell::advance(std::array<AutomataCell*, DIRECTION_COUNT> neighbors)
{
    unsigned aliveNeighbors = 0;

    for (auto* neighbor : neighbors)
    {
        if (neighbor == nullptr) continue;

        auto* conway = static_cast<OrganismCell*>(neighbor);

        if (conway->wasAlive) ++aliveNeighbors;
    }

    canDie = wasAlive;
    canLive = aliveNeighbors >= 1 && !wasAlive;
}

sf::Color OrganismCell::getColor()
{
    return alive ? sf::Color(255, 255, 255) : sf::Color(0, 0, 0);
}

}
