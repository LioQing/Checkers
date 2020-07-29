#pragma once

#include <Vec2.hpp>
#include <Matrix.hpp>
#include <lecs.hpp>

struct Board : public lecs::Component
{
	lio::Vec2i pos;
	int width, height;
	int tile_size;
	lio::Matrixi board;

	Board(const lio::Vec2i& pos, int w, int h, int tile_size) 
		: pos(pos), width(w), height(h), tile_size(tile_size)
	{
		board = lio::Matrixi(w, h, 0);
	}
};