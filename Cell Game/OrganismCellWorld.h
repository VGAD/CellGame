#include "CellWorld.h"
#include "OrganismCell.h"

namespace CellGame
{

class OrganismCellWorld : public CellWorld<OrganismCell>
{
public:
    const float followPercent = 0.1f;

    //! Construct the cell world.
    OrganismCellWorld(unsigned width, unsigned height)
        : CellWorld<OrganismCell>(width, height)
    {}
    
    virtual void init() override;

    //! Perform a single simulation step.
    virtual void step() override;

};

}