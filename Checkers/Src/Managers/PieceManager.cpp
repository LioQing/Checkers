#include "PieceManager.h"

#include <Vec2.hpp>

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void PieceManager::SetBoard(lecs::EntityManager& eman, uint32_t id)
{
	auto& board = game.board->GetComponent<Board>();

	bool up = eman.GetEntity(id).GetComponent<Player>().up;

	for (auto y = 0; y < 3; ++y)
	{
		for (auto x = 0; x < board.width; ++x)
		{
			// Place bottom player pieces
			if ((x + y) % 2 && !up)
			{
				lecs::Entity& piece = eman.AddEntity();
				piece.AddComponent<Piece>(lio::Vec2i(x, y), id);
			}
			// Place top player pieces
			else if (!((x + y) % 2) && up)
			{
				lecs::Entity& piece2 = eman.AddEntity();
				piece2.AddComponent<Piece>(lio::Vec2i(x, game.board->GetComponent<Board>().height - y - 1), id);
			}
		}
	}
}
