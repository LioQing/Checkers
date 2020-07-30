#pragma once

#include <lecs.hpp>

struct Player : public lecs::Component
{
	short col;
	short hl_col;

	bool up;

	Player(short col, short hl_col, bool up)
		: col(col), hl_col(hl_col), up(up)
	{
	}
};