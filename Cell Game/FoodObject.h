#pragma once
#define FOOD_TICKS 120
#include "WorldObject.h"

namespace CellGame
{

class FoodObject :
    public WorldObject
{
public:
    inline FoodObject(int x, int y);

    int ticksLeft = FOOD_TICKS;  //!< Ticks left until this food is set to expire
};

FoodObject::FoodObject(int x, int y)
    : WorldObject(x, y)
{
}

}