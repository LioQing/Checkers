#pragma once

#include <vector>
#include <lecs.hpp>

class PieceManager
{
private:

	std::vector<lecs::Entity*> pieces;

public:

	void SetBoard(lecs::EntityManager& eman, uint32_t id);
};