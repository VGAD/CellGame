#pragma once

#include "AutomataCell.h"
#include <array>

namespace CellGame
{

//! A Conway's Game of Life cell.
class ConwayCell : public AutomataCell
{
public:
    virtual void init() override;
    virtual void update() override;
    virtual void advance(std::array<AutomataCell*, DIRECTION_COUNT> neighbors) override;
    virtual sf::Color getColor() override;

protected:
    bool alive;     //!< Whether this cell is alive.
    bool wasAlive;  //!< Whether this was alive last step.
};

}