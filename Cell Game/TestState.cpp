#include "TestState.h"

namespace CellGame
{

void TestState::update(sf::Time deltaTime)
{

}

void TestState::advance()
{
    cellWorld.step();
}

void TestState::render(float alpha, sf::RenderTarget& renderTarget)
{
    cellWorld.updateTexture();
    renderTarget.draw(sf::Sprite(cellWorld.getTexture()));
}

}