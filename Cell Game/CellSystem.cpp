#include "CellComponent.h"
#include "CellSystem.h"
#include "ECSE/World.h"

namespace CellGame
{

void CellSystem::reset()
{
    sf::IntRect bounds;
    bounds.left = static_cast<int>(size.x * 0.2);
    bounds.width = static_cast<int>(size.x * 0.6);
    bounds.top = static_cast<int>(size.y * 0.2);
    bounds.height = static_cast<int>(size.y * 0.6);

    for (auto x = bounds.left; x < bounds.left + bounds.width; ++x)
    {
        for (auto y = bounds.top; y < bounds.top + bounds.height; ++y)
        {
            addCell(sf::Vector2i(x, y));
        }
    }
}

void CellSystem::addCell(const sf::Vector2i& position)
{
    auto cellId = world->createEntity();
    world->attachComponent<CellComponent>(cellId);

    cells[indexFromPos(position)] = world->registerEntity(cellId);
}

void CellSystem::added()
{
    isAdded = true;
    reset();
}

void CellSystem::update(sf::Time deltaTime)
{
    SetSystem::update(deltaTime);
}

void CellSystem::advance()
{
    SetSystem::advance();
}

void CellSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    auto dot = sf::RectangleShape();
    dot.setFillColor(sf::Color(255, 127, 0));
    dot.setSize(sf::Vector2f(1.f, 1.f));

    for (auto& pair : cells)
    {
        sf::Vector2i pos;
        posFromIndex(pair.first, pos);
        dot.setPosition(static_cast<sf::Vector2f>(pos));

        renderTarget.draw(dot);
    }

    SetSystem::render(alpha, renderTarget);
}

bool CellSystem::checkRequirements(const ECSE::Entity& e) const
{
    // Just return false -- we'll be manually adding and removing cells from the system
    return false;
}

}