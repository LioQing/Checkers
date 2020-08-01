#pragma once

#include <atomic>
#include <LConsoleScreen.hpp>
#include <lecs.hpp>

#include "Managers/Managers.h"

class Game
{
private:

	lio::TConsoleScreen m_tcs;
	lecs::ECSManagers m_ecsman;
	InputManager m_inputman;
	PieceManager m_pieceman;

	std::atomic_bool finished;

	DeltaTime m_delta_time;

public:

	lecs::Entity* board;

	Game();

	void Init(int width, int height, int size = 16, short background_col = 0x0000, bool ten = false);
	void Update(DeltaTime dt);
	void Draw();

	InputManager& InputMan();

	bool HasFinished();
	void Finish();
};