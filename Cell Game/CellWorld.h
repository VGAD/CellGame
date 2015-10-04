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
    bool looping;   //!< If true, cells in this world loop around the edges.

    //! Construct the cell world.
    CellWorld(unsigned width, unsigned height);

    //! Initialize/reset the world.
    virtual void init();

    //! Perform a single simulation step.
    virtual void step();

    //! Update the texture if necessary.
    virtual void updateTexture();

    //! Render to a target
    virtual void render(float alpha, sf::RenderTarget& renderTarget);

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

    //! Get a cell's index from its position.
    /*!
    * \return The index or -1 if invalid position.
    */
    inline unsigned indexFromPos(int x, int y)
    {
        if (looping)
        {
            x = (x + width) % width;
            y = (y + height) % height;
        }
        else
        {
            if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= static_cast<int>(height))
            {
                return -1;
            }
        }

        return x + y * width;
    }

    //! Get a cell's position from its index.
    /*!
    * \param index The cell's index.
    * \param pos Position to place result in.
    * \return true if the index was valid. If false, the position is not changed.
    */
    inline bool posFromIndex(size_t index, sf::Vector2i& pos)
    {
        if (index > width * height) return false;

        pos.x = index % width;
        pos.y = index / width;

        return true;
    }

protected:
    //! Get the neighbor indices to a cell index.
    /*!
    * \param index The cell index.
    * \return An array of neighbor cell indices (or -1 for invalid neighbors).
    */
    std::array<unsigned, AutomataCell::DIRECTION_COUNT> getNeighborIndices(unsigned index);

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
void CellWorld<CellType>::render(float alpha, sf::RenderTarget& renderTarget)
{
    updateTexture();
    renderTarget.draw(sf::Sprite(getTexture()));
}

template <typename CellType>
std::array<unsigned, AutomataCell::DIRECTION_COUNT> CellWorld<CellType>::getNeighborIndices(unsigned index)
{
    std::array<unsigned, AutomataCell::DIRECTION_COUNT> result;

    auto pos = sf::Vector2i(index % width, index / width);

    for (unsigned i = 0; i < offsets.size(); ++i)
    {
        auto neighborPos = pos;
        neighborPos += offsets[i];

        result[i] = indexFromPos(neighborPos.x, neighborPos.y);
    }

    return result;
}

template <typename CellType>
NeighborArray CellWorld<CellType>::getNeighbors(unsigned index)
{
    NeighborArray result;

    auto indices = getNeighborIndices(index);

    for (unsigned i = 0; i < offsets.size(); ++i)
    {
        unsigned neighborIndex = indices[i];

        if (neighborIndex == -1)
        {
            result[i] = nullptr;
            continue;
        }

        result[i] = &cells[neighborIndex];
    }

    return result;
}

}
