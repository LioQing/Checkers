#pragma once

#include <atomic>
#include <LConsoleScreen.hpp>
#include <lecs.hpp>

#include "Managers/InputManager.h"

class Game
{
private:

	lio::TConsoleScreen m_tcs;
	lecs::ECSManagers m_ecsman;
	InputManager m_inputman;

	std::atomic_bool finished;

public:

	lecs::Entity* board;

	Game();

	void Init(int width, int height, int size = 16, short background_col = 0x0000);
	void Update(DeltaTime dt);
	void Draw();

	InputManager& InputMan();

	bool HasFinished();
};