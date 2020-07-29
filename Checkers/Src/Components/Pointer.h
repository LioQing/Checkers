#pragma once

#include <Vec2.hpp>
#include <lecs.hpp>

struct Pointer : public lecs::Component
{
	lio::Vec2<int> pos;
};