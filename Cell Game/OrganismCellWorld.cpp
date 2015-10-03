#include "OrganismCellWorld.h"

namespace CellGame
{

void OrganismCellWorld::init()
{
    CellWorld<OrganismCell>::init();

    sf::IntRect bounds
    {
        static_cast<int>(width * 0.3),
        static_cast<int>(height * 0.3),
        static_cast<int>(width * 0.4),
        static_cast<int>(height * 0.4)
    };

    for (int x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for (int y = bounds.top; y < bounds.top + bounds.height; ++y)
        {
            auto cell = &cells[indexFromPos(x, y)];
            cell->alive = true;
        }
    }
}

void OrganismCellWorld::step()

{
    CellWorld<OrganismCell>::step();

    for (auto& cell : cells) {
        cell.alive = cell.canDie;
    }
}

}