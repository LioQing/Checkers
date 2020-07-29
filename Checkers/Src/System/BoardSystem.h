#pragma once

#include <lecs.hpp>
#include <LConsoleScreen.hpp>

struct BoardSystem : public lecs::System
{
	void LateUpdate(lecs::EntityManager* eman, lecs::EventManager* evman, DeltaTime dt) override;
	void Draw(lecs::EntityManager* eman, lio::TConsoleScreen* tcs) override;
};