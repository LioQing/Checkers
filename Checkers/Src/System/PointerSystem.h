#pragma once

#include <lecs.hpp>

struct PointerSystem : public lecs::System
{
	void EarlyUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt) override;
	void Draw(lecs::EntityManager& eman, lio::TConsoleScreen& tcs) override;
};