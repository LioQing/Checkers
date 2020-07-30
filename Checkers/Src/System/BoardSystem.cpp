#include "BoardSystem.h"

#include <cassert>

#include "../Components/Components.h"

void BoardSystem::LateUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
	// Sync board
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

		// Get active pointer
		Pointer* active_ptr = nullptr;
		for (auto& e2 : eman.EntityFilter<Pointer>().entities)
		{
			auto& pointer = e2->GetComponent<Pointer>();
			if (pointer.active)
			{
				active_ptr = &pointer;
				break;
			}
		}
		assert(active_ptr && "No active pointer found.");

		// Draw board
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
					lio::PIXEL_NONE, col
				);

				// Draw chess
				if (board.board.At(x, y) != 0)
				{
					auto& player = eman.GetEntity(board.board.At(x, y)).GetComponent<Player>();

					if (active_ptr->selected_piece) tcs.DrawString(200, 2, "HL");

					if (active_ptr->selected_piece && active_ptr->selected_piece->GetComponent<Piece>().pos == lio::Vec2i(x, y))
					{
						col += player.hl_col;
					}
					else
					{
						col += player.col;
					}

					tcs.FillCircle(
						board.pos.x + x * board.tile_size + board.tile_size / 2,
						board.pos.y + y * board.tile_size + board.tile_size / 2,
						board.tile_size / 2 - 1,
						lio::PIXEL_SOLID, col
					);
				}
			}
		}

		// Draw possible moves
		auto& player = eman.GetEntity(active_ptr->id).GetComponent<Player>();
		for (auto& pos : active_ptr->possible_moves)
		{
			short bg_col = lio::BG_GREY;
			if ((pos.x + pos.y) % 2) bg_col = lio::BG_DARK_GREY;

			tcs.DrawCircle(
				board.pos.x + pos.x * board.tile_size + board.tile_size / 2,
				board.pos.y + pos.y * board.tile_size + board.tile_size / 2,
				board.tile_size / 2 - 1,
				lio::PIXEL_HALF, player.hl_col + bg_col
			);
		}
	}
}