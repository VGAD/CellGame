#include "OrganismCellWorld.h"
#include "ECSE/Logging.h"

namespace CellGame
{

void OrganismCellWorld::init()
{
    CellWorld<OrganismCell>::init();

    // Set up input
    engine->inputManager.bindInput(0, 0, sf::Keyboard::A, sf::Keyboard::D);
    engine->inputManager.bindInput(1, 0, sf::Keyboard::W, sf::Keyboard::S);

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

    posCursor.x = center.x;
    posCursor.y = center.y;
    negCursor.x = center.x;
    negCursor.y = center.y;
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

        int dx1 = posCursor.x - p1.x,
            dy1 = posCursor.y - p1.y,
            dx2 = posCursor.x - p2.x,
            dy2 = posCursor.y - p2.y;

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

    posCursor.x += engine->inputManager.getIntValue(0);
    posCursor.y += engine->inputManager.getIntValue(1);
}

void OrganismCellWorld::render(float alpha, sf::RenderTarget& renderTarget)
{
    CellWorld<OrganismCell>::render(alpha, renderTarget);

    sf::CircleShape circle(2);
    circle.setFillColor(sf::Color(150, 50, 250));

    sf::Vector2f pos(posCursor);

    circle.setPosition(pos);

    renderTarget.draw(circle);
}

}