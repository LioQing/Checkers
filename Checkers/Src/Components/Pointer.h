#pragma once

#include <array>
#include <vector>
#include <Vec2.hpp>
#include <lecs.hpp>

struct Pointer : public lecs::Component
{
	enum MOVEMENT
	{
		MV_LEFT,
		MV_RIGHT,
		MV_UP,
		MV_DOWN,
	};

	bool active;
	lio::Vec2i pos;

	static const int key_cd = 2.5e5;
	static const int key_cd2 = 5e4;
	std::array<int, 4> cd_count;
	std::array<bool, 4> cd_stage2;

	bool select_key_down;
	lecs::Entity* selected_piece;
	std::vector<lio::Vec2i> possible_moves;

	uint32_t player_id;

	Pointer(lio::Vec2i init_pos, uint32_t id)
		: pos(init_pos), select_key_down(false), selected_piece(nullptr), active(false), player_id(id)
	{
		possible_moves.clear();
		cd_count.fill(0);
		cd_stage2.fill(false);
	}
};