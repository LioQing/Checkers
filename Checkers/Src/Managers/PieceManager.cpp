#include "PieceManager.h"

#include <Vec2.hpp>

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void PieceManager::SetBoard(lecs::EntityManager& eman)
{
	auto& board = game.board->GetComponent<Board>();

	for (auto y = 0; y < 3; ++y)
	{
		for (auto x = 0; x < board.width; ++x)
		{
			if ((x + y) % 2)
			{
				lecs::Entity& piece = eman.AddEntity();
				piece.AddComponent<Piece>(lio::Vec2i(x, y), 1);
			}
		}
	}
}
