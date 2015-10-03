#pragma once

#include "AutomataCell.h"
#include <array>

namespace CellGame
{

//! A cell in the organism game.
class OrganismCell : public AutomataCell
{
public:
    virtual void init() override;
    virtual void update() override;
    virtual void advance(std::array<AutomataCell*, DIRECTION_COUNT> neighbors) override;
    virtual sf::Color getColor() override;

    bool alive;     //!< Whether this cell is alive.
    bool wasAlive;  //!< Whether this was alive last step.

    bool canDie;    //!< Whether this cell can die.
};

}