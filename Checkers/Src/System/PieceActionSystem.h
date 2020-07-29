#pragma once

#include <lecs.hpp>

struct PieceActionSystem : public lecs::System
{
	void EarlyUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt);
};