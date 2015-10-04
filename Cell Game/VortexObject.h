#pragma once
#include "WorldObject.h"

namespace CellGame
{

class VortexObject :
    public WorldObject
{
public:
    inline VortexObject(int x, int y);

    int ticksLeft = 120;  //!< Ticks left until this vortex is set to expire
};

VortexObject::VortexObject(int x, int y)
    : WorldObject(x, y)
{
}

}