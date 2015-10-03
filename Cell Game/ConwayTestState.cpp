#include "ConwayTestState.h"

namespace CellGame
{

void ConwayTestState::update(sf::Time deltaTime)
{

}

void ConwayTestState::advance()
{
    cellWorld.step();
}

void ConwayTestState::render(float alpha, sf::RenderTarget& renderTarget)
{
    cellWorld.updateTexture();
    renderTarget.draw(sf::Sprite(cellWorld.getTexture()));
}

}