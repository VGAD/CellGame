#include "ECSE/WorldState.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/RenderSystem.h"
#include "ECSE/SpriteComponent.h"
#include "ECSE/DepthComponent.h"
#include "ECSE/Engine.h"

namespace CellGame {

//! Tests basic sprite rendering.
class TestState : public ECSE::WorldState
{
public:
    //! Construct the state.
    explicit TestState(ECSE::Engine* engine)
        : ECSE::WorldState(engine)
    {
        world.addSystem<ECSE::RenderSystem>();
        world.addSystem<ECSE::TransformSystem>();
    }

    //! Activate the state.
    virtual void activate() override { LOG(TRACE) << getName() << " active"; }

    //! Deactivate the state.
    virtual void deactivate() override { LOG(TRACE) << getName() << " inactive"; }

    //! Get the state's name.
    virtual std::string getName() override { return "TestState"; }
};

}
