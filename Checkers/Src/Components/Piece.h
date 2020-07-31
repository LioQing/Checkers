#pragma once

#include <Vec2.hpp>
#include <lecs.hpp>

struct Piece : public lecs::Component
{
	lio::Vec2i pos;
	int player_id;
	bool is_king;

	Piece(lio::Vec2i pos, int player_id)
		: pos(pos), player_id(player_id), is_king(false)
	{
	}
};