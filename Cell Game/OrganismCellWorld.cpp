#include "OrganismCellWorld.h"
#include "ECSE/Logging.h"
#include "ECSE/Common.h"
#include "ECSE/VectorMath.h"

namespace CellGame
{

OrganismCellWorld::OrganismCellWorld(ECSE::Engine* _engine, unsigned width, unsigned height)
    : CellWorld<OrganismCell>(width, height), engine(_engine),
    posCursorTex(engine->textureManager.get("Player1.png")), negCursorTex(engine->textureManager.get("Player2.png")),
    foodTex(engine->textureManager.get("food.png"))
{
}

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

    posCursor.x = static_cast<float>(center.x);
    posCursor.y = static_cast<float>(center.y);
    negCursor.x = static_cast<float>(center.x);
    negCursor.y = static_cast<float>(center.y);
}

void OrganismCellWorld::step()
{
    updateBirthChances();
    updateDeathChances();

    CellWorld<OrganismCell>::step();

    redistributeCells();
    calculateFood();
    moveCursor();

    if (engine->inputManager.getIntDelta(5) == 1 && food.size() < 2)
    {
        food.push_back(FoodObject(rand() % width, rand() % height));
        LOG(TRACE) << "Adding food ";// << (*food.rbegin()).getPos().x << " " << (*food.rbegin()).getPos().y;
    }
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

    sf::Sprite foodSpr(foodTex);
    foodSpr.setOrigin(3.f, 3.f);
    for (auto& foodObj : food)
    {
        foodSpr.setPosition(static_cast<sf::Vector2f>(foodObj.getPos()));
        renderTarget.draw(foodSpr);
    }

}

void OrganismCellWorld::moveCursor()
{
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

    // Move slower inside the cell
    if (cells[indexNeg].alive) {
        negCursor += movementNeg * 0.5f;
    }
    // Faster outside of the cell
    else {
        negCursor += movementNeg * 2.f;
    }

    // Mod to wrap
    posCursor.x = static_cast<float>(fmod(posCursor.x + width, width));
    posCursor.y = static_cast<float>(fmod(posCursor.y + height, height));
    negCursor.x = static_cast<float>(fmod(negCursor.x + width, width));
    negCursor.y = static_cast<float>(fmod(negCursor.y + height, height));
}

void OrganismCellWorld::updateBirthChances()
{
    size_t aliveCount = 0;

    for (auto& cell : cells)
    {
        cell.birthChance = 0.f;

        if (cell.alive) ++aliveCount;
    }

    // Below a certain number of cells, disable player control and let the death spiral finish
    if (aliveCount < 200) return;

    const int cursorEffectSize = 30;
    const float strength = 40.f;
    const int foodEffectSize = 50;
    const float foodStrength = 50;

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
            else cells[index].birthChance = ECSE::clamp(0.f, 1.f, strength / (dist * dist));
        }
    }

    // Increase likelihood of cells being born near food
    for (auto& foodObj : food) {
        sf::Vector2i pos = foodObj.getPos();

        for (int x = static_cast<int>(pos.x) - foodEffectSize; x < static_cast<int>(pos.x) + foodEffectSize; ++x)
        {
            for (int y = static_cast<int>(pos.y) - foodEffectSize; y < static_cast<int>(pos.y) + foodEffectSize; ++y)
            {
                size_t index = indexFromPos(x, y);
                float dx1 = ECSE::wrapDifference(static_cast<float>(pos.x), 
                        static_cast<float>(x),
                        static_cast<float>(width)),
                    dy1 = ECSE::wrapDifference(static_cast<float>(pos.y),
                        static_cast<float>(y),
                        static_cast<float>(height));
                float dist = sqrt(dx1 * dx1 + dy1 * dy1);

                if (dist == 0.f) cells[index].birthChance = 1.f;
                else
                {
                    cells[index].birthChance = std::max(cells[index].birthChance, ECSE::clamp(0.f, 1.f, foodStrength / (dist * dist)));
                }
            }
        }
    }
}

void OrganismCellWorld::updateDeathChances()
{
    for (auto& cell : cells)
    {
        cell.deathChance = 0.f;
    }

    const int cursorEffectSize = 30;
    const float strength = 100.f;
    const float maxChance = .40f; // This should be < 1-(cell die chance)

    // Increase likelihood of cells being born near cursor
    for (int x = static_cast<int>(negCursor.x) - cursorEffectSize; x < static_cast<int>(negCursor.x) + cursorEffectSize; ++x)
    {
        for (int y = static_cast<int>(negCursor.y) - cursorEffectSize; y < static_cast<int>(negCursor.y) + cursorEffectSize; ++y)
        {
            size_t index = indexFromPos(x, y);
            float dx1 = ECSE::wrapDifference(negCursor.x, static_cast<float>(x), static_cast<float>(width)),
                dy1 = ECSE::wrapDifference(negCursor.y, static_cast<float>(y), static_cast<float>(height));
            float dist = sqrt(dx1 * dx1 + dy1 * dy1);

            if (dist == 0.f) cells[index].deathChance = maxChance;
            else  {
                cells[index].deathChance = ECSE::clamp(0.f, maxChance, strength / (dist * dist));
            }
        }
    }
}

void OrganismCellWorld::redistributeCells()
{
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

    // DEBUG ADD/REMOVE FOOD
    if (engine->inputManager.getIntValue(4) > 0) toAdd += 100;
    else if (engine->inputManager.getIntValue(4) < 0) toRemove += 100;

    // Accumulate number of dead cells
    toRemove += decay;

    // If we've accumulated enough decay, kill off some cells permanently
    while (toRemove > 1.f && diedIter != died.end())
    {
        cells[*diedIter].alive = false;
        ++diedIter;

        toRemove -= 1.f;
    }

    // If we have cells to add then do so
    while (toAdd > 0 && bornIter != born.end())
    {
        cells[*bornIter].alive = true;
        ++bornIter;

        toAdd -= 1;
    }

    // Redistribute dead cells into born cells
    while (bornIter != born.end() && diedIter != died.end())
    {
        cells[*bornIter].alive = true;
        cells[*diedIter].alive = false;

        ++bornIter;
        ++diedIter;
    }
}

void OrganismCellWorld::calculateFood()
{
    for (auto foodIter = food.begin(); foodIter != food.end(); )
    {
        sf::Vector2i pos = (*foodIter).getPos();

        // Is the tile occupied alive
        if (cells[indexFromPos(pos.x, pos.y)].alive)
        {
            // Do we still have time left
            if ((*foodIter).ticksLeft > 0)
            {
                --(*foodIter).ticksLeft;
                toAdd += feed;
            }
            // Remove
            else
            {
                // "Advance" foodIter
                foodIter = food.erase(foodIter);
                continue;
            }
        }

        // Advance foodIter
        ++foodIter;
    }
}

}