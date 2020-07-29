#pragma once

#include <Vec2.hpp>
#include <lecs.hpp>

struct Piece : public lecs::Component
{
	lio::Vec2<int> pos;
};