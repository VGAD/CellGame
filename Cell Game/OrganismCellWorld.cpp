#include "OrganismCellWorld.h"
#include "ECSE/Logging.h"

namespace CellGame
{

void OrganismCellWorld::init()
{
    CellWorld<OrganismCell>::init();

    sf::Vector2i center
    {
        static_cast<int>(width / 2),
        static_cast<int>(height / 2)
    };

    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            auto cell = &cells[indexFromPos(x, y)];

            int dx = x - center.x;
            int dy = y - center.y;

            cell->alive = sqrt(dx * dx + dy * dy) < width * 0.1;
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
    std::random_shuffle(born.begin(), born.end());
    std::random_shuffle(died.begin(), died.end());

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