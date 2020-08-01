#include "BoardSystem.h"

#include <cassert>

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void BoardSystem::LateUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
	// Check win conditions
	uint32_t players[2] = { 0 };
	for (auto& p : eman.EntityFilter<Piece>().entities)
	{
		// Check still have pieces
		for (auto i = 0; i < 2; ++i)
		{
			if (players[i] == 0)
			{
				players[i] = p->id;
				break;
			}
			else if (players[i] == p->id)
			{
				continue;
			}
		}
	}

	for (auto i = 0; i < 2; ++i)
	{
		if (players[i] == 0)
		{
			game.Finish();
			break;
		}
	}

	// Sync board
	for (auto& e : eman.EntityFilter<Board>().entities)
	{
		auto& board = e->GetComponent<Board>();

		board.board.Fill(0);

		for (auto& e2 : eman.EntityFilter<Piece>().entities)
		{
			auto& piece = e2->GetComponent<Piece>();

			if (piece.is_king)
				board.board.At(piece.pos.x, piece.pos.y) = -piece.player_id;
			else
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
				if (abs(board.board.At(x, y)) != 0)
				{
					auto& player = eman.GetEntity(abs(board.board.At(x, y))).GetComponent<Player>();

					auto c_col = col;
					auto k_col = col;
					if (active_ptr->selected_piece && active_ptr->selected_piece->GetComponent<Piece>().pos == lio::Vec2i(x, y))
					{
						c_col += player.hl_col;
						
						if (player.hl_col == lio::FG_YELLOW)
							k_col += lio::FG_DARK_GREY;
						else
							k_col += lio::FG_YELLOW;
					}
					else
					{
						c_col += player.col;
						if (player.hl_col == lio::FG_YELLOW)
							k_col += lio::FG_BLACK;
						else
							k_col += lio::FG_DARK_YELLOW;
					}

					tcs.FillCircle(
						board.pos.x + x * board.tile_size + board.tile_size / 2,
						board.pos.y + y * board.tile_size + board.tile_size / 2,
						board.tile_size / 2 - 1,
						lio::PIXEL_SOLID, c_col
					);

					if (board.board.At(x, y) < 0)
					{
						tcs.DrawTriangle(
							board.pos.x + x * board.tile_size + board.tile_size / 2,
							board.pos.y + y * board.tile_size + 3 * board.tile_size / 8,
							board.pos.x + x * board.tile_size + 3 * board.tile_size / 8,
							board.pos.y + y * board.tile_size + 5 * board.tile_size / 8,
							board.pos.x + x * board.tile_size + 5 * board.tile_size / 8,
							board.pos.y + y * board.tile_size + 5 * board.tile_size / 8,
							lio::PIXEL_SOLID, k_col);
						tcs.DrawTriangle(
							board.pos.x + x * board.tile_size + 11 * board.tile_size / 16,
							board.pos.y + y * board.tile_size + 3 * board.tile_size / 8,
							board.pos.x + x * board.tile_size + board.tile_size / 2,
							board.pos.y + y * board.tile_size + 5 * board.tile_size / 8,
							board.pos.x + x * board.tile_size + 11 * board.tile_size / 16,
							board.pos.y + y * board.tile_size + 5 * board.tile_size / 8,
							lio::PIXEL_SOLID, k_col);
						tcs.DrawTriangle(
							board.pos.x + x * board.tile_size + 5 * board.tile_size / 16,
							board.pos.y + y * board.tile_size + 3 * board.tile_size / 8,
							board.pos.x + x * board.tile_size + board.tile_size / 2,
							board.pos.y + y * board.tile_size + 5 * board.tile_size / 8,
							board.pos.x + x * board.tile_size + 5 * board.tile_size / 16,
							board.pos.y + y * board.tile_size + 5 * board.tile_size / 8,
							lio::PIXEL_SOLID, k_col);
					}
				}
			}
		}

		// Draw possible moves
		auto& player = eman.GetEntity(active_ptr->player_id).GetComponent<Player>();
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