#include "OrganismState.h"

namespace CellGame
{

OrganismState::OrganismState(ECSE::Engine* engine)
    : ECSE::State(engine), cellWorld(engine, 200, 150)
{
    cellWorld.looping = true;
    cellWorld.init();
}

void OrganismState::update(sf::Time deltaTime)
{

}

void OrganismState::advance()
{
    cellWorld.step();
}

void OrganismState::render(float alpha, sf::RenderTarget& renderTarget)
{
    cellWorld.render(alpha, renderTarget);
}

}