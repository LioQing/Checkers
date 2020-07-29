#include "BoardSystem.h"

#include <cassert>

#include "../Components/Components.h"

void BoardSystem::LateUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
	for (auto& e : eman.EntityFilter<Board>().entities)
	{
		auto& board = e->GetComponent<Board>();

		board.board.Fill(0);

		for (auto& e2 : eman.EntityFilter<Piece>().entities)
		{
			auto& piece = e2->GetComponent<Piece>();

			board.board.At(piece.pos.x, piece.pos.y) = piece.player_id;
		}
	}
}

void BoardSystem::Draw(lecs::EntityManager& eman, lio::TConsoleScreen& tcs)
{
	for (auto& e : eman.EntityFilter<Board>().entities)
	{
		auto& board = e->GetComponent<Board>();

		Pointer* active_ptr = nullptr;
		for (auto& e2 : eman.EntityFilter<Pointer>().entities)
		{
			auto& pointer = e2->GetComponent<Pointer>();
			if (pointer.active)
			{
				active_ptr = &pointer;
			}
		}
		assert(active_ptr && "No active pointer found.");

		for (auto x = 0; x < board.board.width; ++x)
		{
			for (auto y = 0; y < board.board.height; ++y)
			{
				// Get tile checker pattern color
				short col = lio::BG_GREY;
				if ((x + y) % 2) col = lio::BG_DARK_GREY;

				tcs.Fill(
					board.pos.x + x * board.tile_size, board.pos.y + y * board.tile_size,
					board.pos.x + x * board.tile_size + board.tile_size, board.pos.y + y * board.tile_size + board.tile_size,
					lio::PIXEL_NONE, col);

				tcs.DrawString(0, 2, std::to_string(active_ptr->selected_piece != nullptr));

				if (active_ptr->selected_piece && active_ptr->selected_piece->GetComponent<Piece>().pos == lio::Vec2i(x, y))
				{
					col += lio::FG_RED;
				}
				else
				{
					col += lio::FG_DARK_RED;
				}

				if (board.board.At(x, y) == 1)
				{
					tcs.FillCircle(
						board.pos.x + x * board.tile_size + board.tile_size / 2,
						board.pos.y + y * board.tile_size + board.tile_size / 2,
						board.tile_size / 2 - 1,
						lio::PIXEL_SOLID, col);
				}
			}
		}
	}
}