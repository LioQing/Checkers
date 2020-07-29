#pragma once

#include <Matrix.hpp>
#include <lecs.hpp>

struct Board : public lecs::Component
{
	int width, height;
	lio::Matrixi board;

	Board(int w, int h) : width(w), height(h) 
	{
		board = lio::Matrixi(w, h, 0);
	}
};