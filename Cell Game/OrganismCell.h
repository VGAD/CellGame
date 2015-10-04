#pragma once

#include "AutomataCell.h"
#include <array>

// These are duplicated in OrganismCellWorld.h to avoid circular include
#define DIE_CHANCE_IND  0
#define DIE_NEIGH_IND   1
#define ALIVE_NEIGH_IND 2

namespace CellGame
{

//! A cell in the organism game.
class OrganismCell : public AutomataCell
{
public:
    OrganismCell() {}
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
    unsigned int lastFlood; //!< Use by flood fill algorithm to determine whether this has been visited.

    std::vector<int>* mods; //!< Reference to the world's modifier list.
};

}