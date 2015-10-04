#include "OrganismCellWorld.h"
#include "ECSE/Logging.h"
#include "ECSE/Common.h"
#include "ECSE/VectorMath.h"

namespace CellGame
{

OrganismCellWorld::OrganismCellWorld(ECSE::Engine* _engine, unsigned width, unsigned height)
    : CellWorld<OrganismCell>(width, height), engine(_engine),
    posCursorTex(engine->textureManager.get("Player1.png")), negCursorTex(engine->textureManager.get("Player2.png")),
    foodHighTex(engine->textureManager.get("foodHigh.png")), foodLowTex(engine->textureManager.get("foodLow.png")),
    vortexTex(engine->textureManager.get("vortex.png"))
{
}

void OrganismCellWorld::init()
{
    CellWorld<OrganismCell>::init();

    // Set defaults for modifiers
    modifiers.push_back(DIE_CHANCE_DEF);
    modifiers.push_back(DIE_NEIGH_DEF);
    modifiers.push_back(ALIVE_NEIGH_DEF);

    // Give reference to modifiers
    for (auto& cell : cells) {
        cell.mods = &modifiers;
    }

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

            cell->alive = sqrt(dx * dx + dy * dy) < width * 0.15;
        }
    }

    posCursor.x = static_cast<float>(center.x);
    posCursor.y = static_cast<float>(center.y);
    negCursor.x = static_cast<float>(center.x);
    negCursor.y = static_cast<float>(center.y);

    indexOrganisms();
}

void OrganismCellWorld::step()
{
    updateBirthChances();
    updateDeathChances();

    CellWorld<OrganismCell>::step();

    redistributeCells();
    calculateFood();
    calculateVortex();
    moveCursor();

    if (engine->inputManager.getIntDelta(5) == 1 && food.size() < 2)
    {
        food.push_back(FoodObject(rand() % width, rand() % height));
        LOG(TRACE) << "Adding food ";// << (*food.rbegin()).getPos().x << " " << (*food.rbegin()).getPos().y;
    }
    if (engine->inputManager.getIntDelta(6) == 1 && vortices.size() < 2)
    {
        vortices.push_back(VortexObject(rand() % width, rand() % height));
        LOG(TRACE) << "Adding vortex "; // << (*food.rbegin()).getPos().x << " " << (*food.rbegin()).getPos().y;
    }
    if (engine->inputManager.getIntDelta(7))
    {
        modifiers[DIE_NEIGH_IND] = DIE_NEIGH_DEF + (rand() % 3 - 1);
        modifiers[ALIVE_NEIGH_IND] = ALIVE_NEIGH_DEF + (rand() % 3 - 1);
        LOG(TRACE) << modifiers[DIE_NEIGH_IND] << " " << modifiers[ALIVE_NEIGH_IND];
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

    sf::Sprite foodHighSpr(foodHighTex);
    sf::Sprite foodLowSpr(foodLowTex);
    foodHighSpr.setOrigin(3.f, 3.f);
    foodLowSpr.setOrigin(3.f, 3.f);
    for (auto& foodObj : food)
    {
        if (foodObj.ticksLeft > FOOD_TICKS / 2)
        {
            foodHighSpr.setPosition(static_cast<sf::Vector2f>(foodObj.getPos()));
            renderTarget.draw(foodHighSpr);
        }
        else
        {
            foodLowSpr.setPosition(static_cast<sf::Vector2f>(foodObj.getPos()));
            renderTarget.draw(foodLowSpr);
        }
    }

    sf::Sprite vortSpr(vortexTex);
    vortSpr.setOrigin(3.f, 3.f);
    for (auto& vortObj : vortices)
    {
        vortSpr.setPosition(static_cast<sf::Vector2f>(vortObj.getPos()));
        renderTarget.draw(vortSpr);
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
        posCursor += movementPos * 1.5f;
    }
    // Slower outside of the cell
    else {
        posCursor += movementPos * 0.25f;
    }

    // Move slower inside the cell
    if (cells[indexNeg].alive) {
        negCursor += movementNeg * 0.25f;
    }
    // Faster outside of the cell
    else {
        negCursor += movementNeg * 1.5f;
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

    const int cursorEffectSize = 10;
    const float strength = 10.f;

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

    const int foodEffectSize = 15;
    const float foodStrength = 25.f;
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

    const int vortexEffectSize = 10;
    const float vortexStrength = 12.5f;
    // Increase likelihood of cells being born near vortex
    for (auto& vortexObj : vortices) {
        sf::Vector2i pos = vortexObj.getPos();

        for (int x = static_cast<int>(pos.x) - vortexEffectSize; x < static_cast<int>(pos.x) + vortexEffectSize; ++x)
        {
            for (int y = static_cast<int>(pos.y) - vortexEffectSize; y < static_cast<int>(pos.y) + vortexEffectSize; ++y)
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
                    cells[index].birthChance = std::max(cells[index].birthChance, ECSE::clamp(0.f, 1.f, vortexStrength / (dist * dist)));
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

    const int cursorEffectSize = 10;
    const float strength = 10.f;
    const float maxChance = .40f; // This should be < 1-(cell die chance)

    // Increase likelihood of cells being killed near cursor
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

void OrganismCellWorld::indexOrganisms()
{
    for (size_t i = 0; i < cells.size(); ++i)
    {
        auto& cell = cells[i];

        if (!cell.alive)
        {
            cell.organism = 0;
        }
        else if (cell.organism == 0)
        {
            floodFillOrganism(i, false);
        }
    }
}

void OrganismCellWorld::redistributeCells()
{
    size_t indexPosCur = indexFromPos(static_cast<int>(posCursor.x), static_cast<int>(posCursor.y));
    if (cells[indexPosCur].organism != 0)
    {
        playerOrganism = cells[indexPosCur].organism;
    }

    for (auto& cell : cells)
    {
        cell.attached = cell.organism == playerOrganism;
    }

    std::map<unsigned, std::vector<size_t>> born;
    std::map<unsigned, std::vector<size_t>> died;
    std::vector<size_t> changed;
    std::set<unsigned> organisms;

    for (size_t i = 0; i < cells.size(); ++i)
    {
        auto& cell = cells[i];

        if (cell.nextAlive && !cell.alive)
        {
            born[cell.organism].push_back(i);
            organisms.insert(cell.organism);
        }
        else if (!cell.nextAlive && cell.alive)
        {
            died[cell.organism].push_back(i);
            organisms.insert(cell.organism);
        }
    }

    // DEBUG ADD/REMOVE FOOD
    if (engine->inputManager.getIntValue(4) > 0) toAdd += 100;
    else if (engine->inputManager.getIntValue(4) < 0) toRemove += 100;

    // Accumulate number of dead cells
    toRemove += decay;

    for (auto organism : organisms)
    {
        auto& orgBorn = born[organism];
        auto& orgDied = died[organism];

        // Shuffle the list of cells that died/were born
        std::random_shuffle(orgBorn.begin(), orgBorn.end());
        std::random_shuffle(orgDied.begin(), orgDied.end());

        auto bornIter = orgBorn.begin();
        auto diedIter = orgDied.begin();

        if (organism == playerOrganism)
        {
            // If we've accumulated enough decay, kill off some cells permanently
            while (toRemove > 1.f && diedIter != orgDied.end())
            {
                cells[*diedIter].alive = false;
                changed.push_back(*diedIter);
                ++diedIter;

                toRemove -= 1.f;
            }

            // If we have cells to add then do so
            while (toAdd > 0 && bornIter != orgBorn.end())
            {
                cells[*bornIter].alive = true;
                changed.push_back(*bornIter);
                ++bornIter;

                toAdd -= 1;
            }
        }
        else
        {
            // Kill cells randomly
            unsigned killRandom = rand() % 6;
            for (unsigned i = 0; i < killRandom && diedIter != orgDied.end(); ++i)
            {
                cells[*diedIter].alive = false;
                changed.push_back(*diedIter);
                ++diedIter;
            }
        }

        // Redistribute dead cells into born cells
        while (bornIter != orgBorn.end() && diedIter != orgDied.end())
        {
            cells[*bornIter].alive = true;
            cells[*diedIter].alive = false;

            changed.push_back(*bornIter);
            changed.push_back(*diedIter);

            ++bornIter;
            ++diedIter;
        }
    }

    std::random_shuffle(changed.begin(), changed.end());

    for (auto& index : changed)
    {
        cells[index].organism = 0;

        for (auto neighbor : getNeighborIndices(index))
        {
            cells[neighbor].organism = 0;
        }
    }

    for (auto it = changed.begin(); it != changed.end(); ++it)
    {
        auto index = *it;
        auto& cell = cells[index];

        // Newly-created; find nearby organisms
        if (cell.alive)
        {
            if (cell.organism == 0) floodFillOrganism(index, true);
        }
        // Newly-destroyed; neighbors may have split into new organisms
        else
        {
            for (auto neighborIndex : getNeighborIndices(index))
            {
                auto& neighbor = cells[neighborIndex];

                if (neighbor.alive && neighbor.organism == 0) floodFillOrganism(neighborIndex, false);
            }
        }
    }
}

void OrganismCellWorld::floodFillOrganism(unsigned int startCell, bool find)
{
    static unsigned int floodId = 0;
    ++floodId;

    std::vector<unsigned int> open;
    std::vector<unsigned int> closed;
    open.push_back(startCell);

    unsigned int foundOrganism = 0;

    while (!open.empty())
    {
        unsigned int index = open.back();
        open.pop_back();
        closed.push_back(index);

        auto& cell = cells[index];
        cell.lastFlood = floodId;

        if (!cell.alive) continue;

        if (find)
        {
            // Found an organism
            if (cell.organism != 0)
            {
                // Hadn't found one yet, so store it
                if (foundOrganism == 0)
                {
                    foundOrganism = cell.organism;
                }
                // Two organisms touching; pick the lower index
                else if (cell.organism < foundOrganism)
                {
                    foundOrganism = cell.organism;
                }
            }
        }

        for (auto neighbor : getNeighborIndices(index))
        {
            if (cells[neighbor].lastFlood == floodId) continue;
            open.push_back(neighbor);
        }
    }

    // New organism if nothing else was touching (or not trying to find an organism)
    if (foundOrganism == 0)
    {
        foundOrganism = nextOrgIndex;
        ++nextOrgIndex;
    }

    // Set the organism index of everything touching
    for (auto& index : closed)
    {
        cells[index].organism = foundOrganism;
    }
}

void OrganismCellWorld::calculateFood()
{
    for (auto foodIter = food.begin(); foodIter != food.end(); )
    {
        sf::Vector2i pos = (*foodIter).getPos();

        auto& cell = cells[indexFromPos(pos.x, pos.y)];

        // Is the tile occupied alive and part of the player
        if (cell.alive && cell.attached)
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


void OrganismCellWorld::calculateVortex()
{
    for (auto vortexIter = vortices.begin(); vortexIter != vortices.end();)
    {
        sf::Vector2i pos = (*vortexIter).getPos();

        // Is the tile occupied alive
        if (cells[indexFromPos(pos.x, pos.y)].alive)
        {
            // Do we still have time left
            if ((*vortexIter).ticksLeft > 0)
            {
                --(*vortexIter).ticksLeft;
                toRemove += suck;
            }
            // Remove
            else
            {
                // "Advance" vortexIter
                vortexIter = vortices.erase(vortexIter);
                continue;
            }
        }

        // Advance vortexIter
        ++vortexIter;
    }
}
}