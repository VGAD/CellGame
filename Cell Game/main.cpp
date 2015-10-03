#include "ECSE/Engine.h"
#include "ECSE/Common.h"
#include "ECSE/Logging.h"
#include "TestState.h"

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
        ECSE::Engine engine(sf::Vector2i(800, 600), 1.f, "Cell Game");

        engine.pushState<TestState>();

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