#pragma once

#include "ECSE/Engine.h"
#include "CellWorld.h"
#include "OrganismCell.h"

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

private:
    //! Engine pointer to get access to input
    ECSE::Engine* engine;   

    //! Vectors representing current cursor positions
    sf::Vector2i posCursor, negCursor;

    //! Graphics for cursors
    const sf::Texture& posCursorTex, negCursorTex;
};

}