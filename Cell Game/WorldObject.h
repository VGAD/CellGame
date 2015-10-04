#pragma once

#include <SFML/Graphics.hpp>

namespace CellGame
{

class WorldObject
{
public:
    inline WorldObject(int x, int y);

    inline virtual sf::Vector2i getPos() const { return pos; }


private:
    sf::Vector2i pos;
};

WorldObject::WorldObject(int x, int y)
    : pos(x, y)
{
}

}