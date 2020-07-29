#pragma once

#include <LConsoleScreen.hpp>
#include <lecs.hpp>

class Game
{
private:

	lio::TConsoleScreen m_tcs;
	lecs::EntityManager m_entity_man;

	bool finished;

public:

	Game();

	bool HasFinished();

	void Init(int width, int height, int size = 16, short background_col = 0x0000);
	void Update(DeltaTime dt);
	void Draw();
};