#pragma once

#include <LConsoleScreen.hpp>

class Game
{
private:

	lio::LConsoleScreen m_lcs;

public:

	Game();

	void Init(int width, int height, int size = 16, short background_col = 0x0000);
	void Update();
	void Draw();
};