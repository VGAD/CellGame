#include "ECSE/WorldState.h"
#include "ECSE/Engine.h"
#include "ECSE/Logging.h"
#include "OrganismCellWorld.h"

namespace CellGame
{

//! State with an organism world.
class OrganismState : public ECSE::State
{
public:
    //! Construct the state.
    explicit OrganismState(ECSE::Engine* engine);

    virtual void update(sf::Time deltaTime) override;
    virtual void advance() override;
    virtual void render(float alpha, sf::RenderTarget& renderTarget) override;

    //! Activate the state.
    virtual void activate() override { LOG(TRACE) << getName() << " active"; }

    //! Deactivate the state.
    virtual void deactivate() override { LOG(TRACE) << getName() << " inactive"; }

    //! Get the state's name.
    virtual std::string getName() override { return "TestState"; }

private:
    OrganismCellWorld cellWorld;
};

}
