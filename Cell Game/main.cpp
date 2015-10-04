#include "ECSE/Engine.h"
#include "ECSE/Common.h"
#include "ECSE/Logging.h"
#include "OrganismState.h"
#include "CellEngine.h"

//! Contains all CellGame-specific code.
namespace CellGame {}

// Memory leak debug
#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

int main(int argv, char* argc[])
{
    using namespace CellGame;

#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG)
    // Memory leak debug
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

    ECSE::init_logging(argv, argc);

    try
    {
        // Run the game
        CellEngine engine(sf::Vector2i(200, 150), 4.f, "Cell Game");

        // Set up input for posCursor
        engine.inputManager.bindInput(0, 0, sf::Keyboard::A, sf::Keyboard::D);
        engine.inputManager.bindInput(1, 0, sf::Keyboard::W, sf::Keyboard::S);

        // Set up input for negCursor
        engine.inputManager.bindInput(2, 0, sf::Keyboard::Left, sf::Keyboard::Right);
        engine.inputManager.bindInput(3, 0, sf::Keyboard::Up, sf::Keyboard::Down);

        engine.inputManager.bindInput(4, 0, sf::Keyboard::SemiColon, sf::Keyboard::P); // Shrink/Grow
        engine.inputManager.bindInput(5, 0, sf::Keyboard::BackSlash); // Food
        engine.inputManager.bindInput(6, 0, sf::Keyboard::RBracket); // Vortex

        engine.pushState<OrganismState>();

        engine.run();
    }
    catch (const std::runtime_error& e)
    {
        LOG(FATAL) << e.what();
        ECSE::errorMessage(e.what());
    }

    LOG(INFO) << "Exiting CellGame";

    return 0;
}