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

    sf::Vector2i center
    {
        static_cast<int>(width / 2),
        static_cast<int>(height / 2)
    };

    for (int x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for (int y = bounds.top; y < bounds.top + bounds.height; ++y)
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

    std::random_shuffle(died.begin(), died.end());
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
    }
        );


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