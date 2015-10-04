#include "ECSE/Logging.h"
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

    int dieChance = (*mods)[DIE_CHANCE_IND] + static_cast<int>(100 * deathChance);
    bool randomDie = (rand() % 100 < dieChance) &&
        (aliveNeighbors < (*mods)[DIE_NEIGH_IND]);

    bool randomSpawn = (rand() % 100 < static_cast<int>(100 * birthChance)) &&
        (aliveNeighbors > (*mods)[ALIVE_NEIGH_IND]);

    nextAlive = (aliveNeighbors > 2 && !randomDie) || randomSpawn;
}

sf::Color OrganismCell::getColor()
{
    if (alive) return attached ? sf::Color(60, 110, 30) : sf::Color(110, 60, 30);

    return sf::Color(0, 0, 0);
}

}
