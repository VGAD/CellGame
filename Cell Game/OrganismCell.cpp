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

    bool randomDie = (rand() % 100 > 30) && (aliveNeighbors < 6);
    nextAlive = aliveNeighbors > 2 && !randomDie;
}

sf::Color OrganismCell::getColor()
{
    if (alive) return sf::Color(60, 110, 30);

    return sf::Color(0, 0, 0);
}

}
