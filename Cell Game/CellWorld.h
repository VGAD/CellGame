#pragma once

#include <type_traits>
#include <SFML/Graphics.hpp>
#include "AutomataCell.h"
#include <array>

namespace CellGame
{

//! Holds an array of cells adjacent to a cell.
using NeighborArray = std::array<AutomataCell*, AutomataCell::DIRECTION_COUNT>;

//! A cellular automata world which can be rendered.
template <typename CellType>
class CellWorld
{
    static_assert(std::is_base_of<AutomataCell, CellType>::value, "Cell type must be derived from AutomataCell");

public:
    //! Construct the cell world.
    CellWorld(unsigned width, unsigned height);

    //! Initialize/reset the world.
    void init();

    //! Perform a single simulation step.
    void step();

    //! Update the texture if necessary.
    void updateTexture();

    //! Get the texture for rendering.
    /*!
    * \return The render texture.
    */
    inline const sf::Texture& getTexture() { return texture; }

    //! Get world's width.
    /*!
    * \return The cell world's width.
    */
    inline unsigned getWidth() const { return width; }

    //! Get world's height.
    /*!
    * \return The cell world's height.
    */
    unsigned getHeight() const { return height; }

private:
    //! Get the neighbors to a cell index.
    /*!
    * \param index The cell index.
    * \return An array of neighbor cells (or nullptr for invalid neighbors).
    */
    NeighborArray getNeighbors(unsigned index);

    unsigned width;                 //!< The world's width in cells.
    unsigned height;                //!< The world's height in cells.
    bool toUpdateTexture = false;   //!< Whether the texture needs to be updated before rendering.
    sf::Texture texture;            //!< The texture to which cells are rendered.
    std::vector<sf::Uint8> pixels;  //!< The pixel buffer.
    std::vector<CellType> cells;    //!< Vector containing cell data.

    //! Tile offsets for each of the neighbor directions.
    using OffsetArray = std::array<sf::Vector2i, AutomataCell::DIRECTION_COUNT>;
    const OffsetArray offsets = OffsetArray{
        sf::Vector2i(1, 0),
        sf::Vector2i(1, 1),
        sf::Vector2i(0, 1),
        sf::Vector2i(-1, 1),
        sf::Vector2i(-1, 0),
        sf::Vector2i(-1, -1),
        sf::Vector2i(0, -1),
        sf::Vector2i(1, -1)
    };
};

template <typename CellType>
CellWorld<CellType>::CellWorld(unsigned width, unsigned height)
    : width(width), height(height)
{
    texture.create(width, height);

    auto area = width * height;
    pixels.resize(4 * area); // RGBA pixels = 4 Uint8 per pixel
    cells.resize(area);

    init();
}

template <typename CellType>
void CellWorld<CellType>::init()
{
    for (auto& cell : cells)
    {
        cell.init();
    }

    toUpdateTexture = true;
}

template <typename CellType>
void CellWorld<CellType>::step()
{
    for (auto& cell : cells)
    {
        cell.update();
    }

    for (unsigned i = 0; i < cells.size(); ++i)
    {
        cells[i].advance(getNeighbors(i));
    }

    toUpdateTexture = true;
}

template <typename CellType>
void CellWorld<CellType>::updateTexture()
{
    if (!toUpdateTexture) return;

    for (unsigned i = 0; i < cells.size(); ++i)
    {
        auto color = cells[i].getColor();
        auto offset = i * 4;

        pixels[offset] = color.r;
        pixels[offset + 1] = color.g;
        pixels[offset + 2] = color.b;
        pixels[offset + 3] = color.a;
    }

    texture.update(&pixels[0]);

    toUpdateTexture = false;
}

template <typename CellType>
NeighborArray CellWorld<CellType>::getNeighbors(unsigned index)
{
    NeighborArray result;

    auto pos = sf::Vector2i(index % width, index / width);

    for (unsigned i = 0; i < offsets.size(); ++i)
    {
        auto neighborPos = pos;
        neighborPos += offsets[i];

        if (neighborPos.x < 0 || neighborPos.x >= static_cast<int>(width) ||
            neighborPos.y < 0 || neighborPos.y >= static_cast<int>(height))
        {
            result[i] = nullptr;
            continue;
        }

        unsigned neighborIndex = neighborPos.x + neighborPos.y * width;
        result[i] = &cells[neighborIndex];
    }

    return result;
}

}
