#pragma once

#include "ECSE/Engine.h"
#include "CellWorld.h"
#include "OrganismCell.h"
#include "FoodObject.h"
#include "VortexObject.h"

#define DIE_CHANCE_DEF  60
#define DIE_NEIGH_DEF   6
#define ALIVE_NEIGH_DEF 2

// These are duplicated in OrganismCell.h to avoid circular include
#define DIE_CHANCE_IND  0
#define DIE_NEIGH_IND   1
#define ALIVE_NEIGH_IND 2


namespace CellGame
{

class OrganismCellWorld : public CellWorld<OrganismCell>
{
public:
    const float followPercent = 0.1f;

    //! Construct the cell world.
    OrganismCellWorld(ECSE::Engine* _engine, unsigned width, unsigned height);
    
    virtual void init() override;

    //! Perform a single simulation step.
    virtual void step() override;

    virtual void render(float alpha, sf::RenderTarget& renderTarget);

    //! Move the cursor positions.
    virtual void moveCursor();

    //! Update chance of cells being born on organism edges.
    virtual void updateBirthChances();

    //! Update chance of cells being killed on organism edges.
    virtual void updateDeathChances();

    //! Assign ids to each organism.
    virtual void indexOrganisms();

    //! Redistribute dead and born cells.
    virtual void redistributeCells();

    //! Flood fill cells with an organism index.
    /*!
    * \param startCell The cell from which to start the flood fill.
    * \param find If true, will try to use the index of an existing organism. If false, will create a new index.
    */
    virtual void floodFillOrganism(unsigned int startCell, bool find);

    //! Calculate how food affects the cells.
    virtual void calculateFood();

    //! Calculate how vortices affects the cells
    virtual void calculateVortex();

private:
    //! Up to this many dead cells will be removed with no replacement each frame.
    const float decay = 1.2f;

    //! Add this many cells per tick that food is feeding the blob
    const int feed = 7;
    
    //! Remove this many cells per tick that vortex is killing the blob
    const int suck = 2;

    //! Engine pointer to get access to input
    ECSE::Engine* engine;   

    //! Vectors representing current cursor positions
    sf::Vector2f posCursor, negCursor;

    //! Graphics for cursors and food and vortices
    const sf::Texture& posCursorTex, negCursorTex, foodHighTex, foodLowTex, vortexTex;

    //! The number of cells to remove this frame.
    /*!
    * This accumulates, and when it reaches at least 1, a cell is removed.
    */
    float toRemove = 0.f;
    
    //! The number of cells to add as soon as possible.
    /*!
    * This tries to be taken to 0 as soon as possible.
    */
    int toAdd = 0;

    //! The index of the player's organism.
    unsigned int playerOrganism;

    //! The next organism index to use.
    unsigned int nextOrgIndex = 1;

    //! Vector of currect food locations
    std::deque<FoodObject> food;

    //! Vector of currect vortex locations
    std::deque<VortexObject> vortices;
    
    //! Vector of cell modifiers
    std::vector<int> modifiers;

    //! Timer for spawning food.
    unsigned int foodTimer = 0;
};

}