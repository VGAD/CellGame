#pragma once

#include "ECSE/SetSystem.h"
#include <map>

namespace CellGame {

//! A System that updates and renders cells.
class CellSystem : public ECSE::SetSystem
{
public:
    //! Construct the CellSystem.
    explicit CellSystem(ECSE::World* system)
        : SetSystem(system)
    {
    }

    //! Set the size of the cell system.
    /*!
    * \param size The new size.
    */
    void setSize(sf::Vector2i size)
    {
        this->size = size;
        if (isAdded) reset();
    }

    //! Get the size of the cell system.
    /*!
    * \return The size of the cell system.
    */
    sf::Vector2i getSize() const
    {
        return size;
    }

    //! Reset the cell system to its initial state.
    void reset();

    //! Add a cell to the system.
    /*!
    * \param position The position of the new cell.
    */
    void addCell(const sf::Vector2i& position);

    //! Convert a position to a cell index.
    /*!
    * \param position The cell position.
    * \return The position index, or -1 if the position is invalid.
    */
    inline int indexFromPos(const sf::Vector2i& position) const
    {
        if (position.x < 0 || position.x >= size.x ||
            position.y < 0 || position.y >= size.y)
        {
            return -1;
        }

        return position.x + position.y * size.x;
    }

    //! Convert a cell index to a position.
    /*!
    * \param index The cell index.
    * \param position A reference to the position to update.
    * \return true if the index was valid, else false. if false, the position is not set.
    */
    inline bool posFromIndex(unsigned int index, sf::Vector2i& position) const
    {
        if (index >= getMaxCells())
        {
            return false;
        }

        position.x = index % size.x;
        position.y = index / size.x;

        return true;
    }

    //! Get the maximum number of cells in the system.
    /*!
    * \return The maximum number of cells.
    */
    unsigned int getMaxCells() const
    {
        return size.x * size.y;
    }

    virtual void added() override;
    virtual void update(sf::Time deltaTime) override;
    virtual void advance() override;
    virtual void render(float alpha, sf::RenderTarget& renderTarget) override;
    bool checkRequirements(const ECSE::Entity& e) const override;

private:
    sf::Vector2i size;                              //!< The size of the cell system.
    std::map<unsigned int, ECSE::Entity*> cells;    //!< Map from cell position index to cell entity.
    bool isAdded = false;                           //!< Whether this has been added to the world yet.
};

}