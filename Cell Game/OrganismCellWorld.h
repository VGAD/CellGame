#include "CellWorld.h"
#include "OrganismCell.h"

namespace CellGame
{

class OrganismCellWorld : public CellWorld<OrganismCell>
{
public:
    //! Construct the cell world.
    OrganismCellWorld(unsigned width, unsigned height)
        : CellWorld<OrganismCell>(width, height)
    {}

    virtual void init() override;
};

}