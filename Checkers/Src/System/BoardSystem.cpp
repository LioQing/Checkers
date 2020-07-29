#include "BoardSystem.h"

#include "../Components/Components.h"

void BoardSystem::LateUpdate(lecs::EntityManager* eman, lecs::EventManager* evman, DeltaTime dt)
{
	for (auto& e : eman->EntityFilter<Board>().entities)
	{

	}
}

void BoardSystem::Draw(lecs::EntityManager* eman, lio::TConsoleScreen* tcs)
{
	for (auto& e : eman->EntityFilter<Board>().entities)
	{
		auto& board = e->GetComponent<Board>();

		for (auto x = 0; x < board.board.width; ++x)
		{
			for (auto y = 0; y < board.board.height; ++y)
			{
				// Get tile checker pattern color
				short col = lio::BG_GREY;
				if ((x + y) % 2) col = lio::BG_DARK_GREY;

				if (board.board.At(x, y) == 0)
				{
					tcs->Fill(
						board.pos.x + x * board.tile_size, board.pos.y + y * board.tile_size,
						board.pos.x + x * board.tile_size + board.tile_size, board.pos.y + y * board.tile_size + board.tile_size,
						lio::PIXEL_NONE, col);
				}
			}
		}
	}
}