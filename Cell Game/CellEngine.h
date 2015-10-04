#pragma once
#include "ECSE/Engine.h"
#include <fstream>

namespace CellGame
{

//! Specialized Engine for CellGame.
class CellEngine : public ECSE::Engine
{
public:
    //! Construct the engine.
    explicit CellEngine(sf::Vector2i size, std::string name = "", unsigned int fps = 60,
                        bool noRender = false, bool lockstep = false)
                        : Engine(size, name, fps, noRender, lockstep)
    {
    }

    //! Construct the engine with a scaled window.
    explicit CellEngine(sf::Vector2i size, float scale, std::string name = "", unsigned int fps = 60,
                        bool noRender = false, bool lockstep = false)
                        : Engine(size, scale, name, fps, noRender, lockstep)
    {
    }

    virtual ~CellEngine()
    {
    }

protected:
    //! Handle events fromt he SFML window.
    virtual void handleEvent(const sf::Event& e) override;
};

}