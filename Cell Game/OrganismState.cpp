#include "OrganismState.h"

namespace CellGame
{

OrganismState::OrganismState(ECSE::Engine* engine)
    : ECSE::State(engine), cellWorld(engine, 400, 300)
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
    cellWorld.updateTexture();
    renderTarget.draw(sf::Sprite(cellWorld.getTexture()));
}

}