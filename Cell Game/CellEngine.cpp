#include "CellEngine.h"
#include "OrganismState.h"

namespace CellGame
{

void CellEngine::handleEvent(const sf::Event& e)
{
    switch (e.type)
    {
    case sf::Event::KeyPressed:
        if (e.key.code == sf::Keyboard::R)
        {
            popState();
            pushState<OrganismState>();
        }
        break;

    default:
        break;
    }

    Engine::handleEvent(e);
}

}