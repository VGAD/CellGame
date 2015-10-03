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

    std::vector<size_t> born;
    std::vector<size_t> died;

    for (size_t i = 0; i < cells.size(); ++i)
    {
        if (cells[i].nextAlive && !cells[i].alive)
        {
            born.push_back(i);
        }
        else if (!cells[i].nextAlive && cells[i].alive)
        {
            died.push_back(i);
        }
    }
    
    // Shuffle the list of cells that died
    std::random_shuffle(died.begin(), died.end());

    // Sort the list of cells that will be born to move towards an objective
    std::sort(born.begin(), born.end(), 
        [&](size_t a, size_t b) -> bool
    {
        sf::Vector2i p1, p2;
        posFromIndex(a, p1);
        posFromIndex(b, p2);

        // TEMPORARY DESTINATION
        sf::Vector2i dest(0, 0);

        int dx1 = dest.x - p1.x,
            dy1 = dest.y - p1.y,
            dx2 = dest.x - p2.x,
            dy2 = dest.y - p2.y;

        return (dx1*dx1 + dy1*dy1) < (dx2*dx2 + dy2*dy2);
    });

    auto bornIter = born.begin();
    auto diedIter = died.begin();

    while (bornIter != born.end() && diedIter != died.end())
    {
        cells[*bornIter].alive = true;
        cells[*diedIter].alive = false;

        ++bornIter;
        ++diedIter;
    }
}

}