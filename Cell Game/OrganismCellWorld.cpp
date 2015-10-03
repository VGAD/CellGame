#include "OrganismCellWorld.h"
#include "ECSE/Logging.h"

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
            cell->alive = (rand() % 10) > 2;
        }
    }
}

void OrganismCellWorld::step()
{
    CellWorld<OrganismCell>::step();

    std::vector<OrganismCell*> born;
    std::vector<OrganismCell*> died;

    for (auto& cell : cells)
    {
        if (cell.nextAlive && !cell.alive)
        {
            born.push_back(&cell);
        }
        else if (!cell.nextAlive && cell.alive)
        {
            died.push_back(&cell);
        }
    }

    auto bornIter = born.begin();
    auto diedIter = died.begin();

    while (bornIter != born.end() && diedIter != died.end())
    {
        (*bornIter)->alive = true;
        (*diedIter)->alive = false;

        ++bornIter;
        ++diedIter;
    }
}

}