#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace CellGame
{

//! A single cell in a cell world.
class AutomataCell
{
public:
    //! Possible directions of neighbor cells.
    enum Direction
    {
        RIGHT,
        DOWN_RIGHT,
        DOWN,
        DOWN_LEFT,
        LEFT,
        UP_LEFT,
        UP,
        UP_RIGHT,

        DIRECTION_COUNT
    };

    //! Destroy the cell.
    virtual ~AutomataCell() {}

    //! Initialize the cell.
    virtual void init() {};

    //! Prepare state for the upcoming step.
    virtual void update() {};

    //! Change cell state based on neighbors.
    /*!
    * \param neighbors The neighbor cells, or nullptr for invalid neighbors.
    */
    virtual void advance(std::array<AutomataCell*, DIRECTION_COUNT> neighbors) {};

    //! Get the cell's color.
    /*!
    * \return The cell's color.
    */
    virtual sf::Color getColor() { return sf::Color(); }
};

}