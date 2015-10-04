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

    float birthChance;      //!< Likelihood of a random birth on organism edge.
    float deathChance;      //!< Likelihood of a random death on organism edge.
    bool alive;             //!< Whether this cell is alive.
    bool nextAlive;         //!< Whether the cell could be alive next step.
    bool prevAlive;         //!< Whether this was alive last step.
    bool attached;          //!< Whether this cell is attached to the player organism.
    unsigned int organism;  //!< The organism to which this belongs.
};

}