#include "OrganismCellWorld.h"
#include "ECSE/Logging.h"
#include "ECSE/Common.h"
#include "ECSE/VectorMath.h"

namespace CellGame
{

OrganismCellWorld::OrganismCellWorld(ECSE::Engine* _engine, unsigned width, unsigned height)
    : CellWorld<OrganismCell>(width, height), engine(_engine),
    posCursorTex(engine->textureManager.get("Player1.png")), negCursorTex(engine->textureManager.get("Player2.png"))
{
}

void OrganismCellWorld::init()
{
    CellWorld<OrganismCell>::init();

    // Set up input for posCursor
    engine->inputManager.bindInput(0, 0, sf::Keyboard::A, sf::Keyboard::D);
    engine->inputManager.bindInput(1, 0, sf::Keyboard::W, sf::Keyboard::S);
    
    // Set up input for negCursor
    engine->inputManager.bindInput(2, 0, 
        sf::Keyboard::Left, sf::Keyboard::Right);
    engine->inputManager.bindInput(3, 0, 
        sf::Keyboard::Up, sf::Keyboard::Down);

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

    posCursor.x = static_cast<float>(center.x);
    posCursor.y = static_cast<float>(center.y);
    negCursor.x = static_cast<float>(center.x);
    negCursor.y = static_cast<float>(center.y);
}

void OrganismCellWorld::step()
{
    for (auto& cell : cells)
    {
        cell.birthChance = 0.f;
    }

    const int cursorEffectSize = 20;

    // Increase likelihood of cells being born near cursor
    for (int x = static_cast<int>(posCursor.x) - cursorEffectSize; x < static_cast<int>(posCursor.x) + cursorEffectSize; ++x)
    {
        for (int y = static_cast<int>(posCursor.y) - cursorEffectSize; y < static_cast<int>(posCursor.y) + cursorEffectSize; ++y)
        {
            size_t index = indexFromPos(x, y);
            float dx1 = ECSE::wrapDifference(posCursor.x, static_cast<float>(x), static_cast<float>(width)),
                  dy1 = ECSE::wrapDifference(posCursor.y, static_cast<float>(y), static_cast<float>(height));
            float dist = sqrt(dx1 * dx1 + dy1 * dy1);
            
            if (dist == 0.f) cells[index].birthChance = 1.f;
            else cells[index].birthChance = ECSE::clamp(0.f, 1.f, 20.f / (dist * dist));
        }
    }

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
    
    // Shuffle the list of cells that died/were born
    std::random_shuffle(died.begin(), died.end());
    std::random_shuffle(born.begin(), born.end());

    auto bornIter = born.begin();
    auto diedIter = died.begin();

    while (bornIter != born.end() && diedIter != died.end())
    {
        cells[*bornIter].alive = true;
        cells[*diedIter].alive = false;

        ++bornIter;
        ++diedIter;
    }

    // Get input
    float dxPos = engine->inputManager.getFloatValue(0),
        dyPos = engine->inputManager.getFloatValue(1),
        dxNeg = engine->inputManager.getFloatValue(2),
        dyNeg = engine->inputManager.getFloatValue(3);

    // 5 = width and height of cursor sprite
    int indexPos = indexFromPos(static_cast<int>(posCursor.x + dxPos * 5.f), static_cast<int>(posCursor.y + dyPos * 5.f)),
        indexNeg = indexFromPos(static_cast<int>(negCursor.x + dxPos * 5.f), static_cast<int>(negCursor.y + dyPos * 5.f));

    auto movementPos = sf::Vector2f(dxPos, dyPos),
        movementNeg = sf::Vector2f(dxNeg, dyNeg);
    ECSE::normalize(movementPos);
    ECSE::normalize(movementNeg);

    // Move at full speed inside the cell
    if (cells[indexPos].alive) {
        posCursor += movementPos * 2.f;
    }
    // Slower outside of the cell
    else {
        posCursor += movementPos * 0.5f;
    }

    // Move at full speed inside the cell
    if (cells[indexNeg].alive) {
        negCursor += movementNeg * 2.f;
    }
    // Slower outside of the cell
    else {
        negCursor += movementNeg * 0.5f;
    }

    posCursor.x = static_cast<float>(fmod(posCursor.x + width, width));
    posCursor.y = static_cast<float>(fmod(posCursor.y + height, height));
    negCursor.x = static_cast<float>(fmod(negCursor.x + width, width));
    negCursor.y = static_cast<float>(fmod(negCursor.y + height, height));
}

void OrganismCellWorld::render(float alpha, sf::RenderTarget& renderTarget)
{
    CellWorld<OrganismCell>::render(alpha, renderTarget);

    sf::Sprite posCursorSpr(posCursorTex);
    sf::Sprite negCursorSpr(negCursorTex);

    posCursorSpr.setOrigin(3.f, 3.f);
    negCursorSpr.setOrigin(3.f, 3.f);
    
    posCursorSpr.setPosition(
        floor(posCursor.x),
        floor(posCursor.y));
    negCursorSpr.setPosition(
        floor(negCursor.x),
        floor(negCursor.y));

    renderTarget.draw(posCursorSpr);
    renderTarget.draw(negCursorSpr);
}

}