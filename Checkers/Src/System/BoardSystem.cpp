#include "BoardSystem.h"

#include <cassert>

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void BoardSystem::LateUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
	// Check win conditions
	auto& board = game.board->GetComponent<Board>();

	auto IsPossMv = [&board](Piece& piece, int x, int y)
	{
		if (x > 0 && piece.pos.x + x >= board.board.width) return false;
		if (x < 0 && piece.pos.x + x < 0) return false;
		if (y > 0 && piece.pos.y + y >= board.board.height) return false;
		if (y < 0 && piece.pos.y + y < 0) return false;
		if (board.board.At(piece.pos.x + x, piece.pos.y + y) != 0) return false;

		return true;
	};

	uint32_t players[2] = { 0 };
	for (auto& p : eman.EntityFilter<Piece>().entities)
	{
		auto& piece = p->GetComponent<Piece>();
		auto& player = eman.GetEntity(piece.player_id).GetComponent<Player>();

		lio::Vec2i mv(piece.pos.x + 1, piece.pos.y + (player.up ? -1 : 1));
		auto PushPossMv = [&](bool left, bool backward)
		{
			if (IsPossMv(piece, 1 * (left ? -1 : 1), (backward == player.up ? -1 : 1)))
			{
				return true;
			}
			else if (IsPossMv(piece, 2 * (left ? -1 : 1), (backward == player.up ? -2 : 2)) && abs(board.board.At(mv.x, mv.y)) != piece.player_id && abs(board.board.At(mv.x, mv.y)) != 0)
			{
				return true;
			}
			return false;
		};

		auto CanMv = [&]()
		{
			if (PushPossMv(false, true)) return true;

			mv.x -= 2;
			if (PushPossMv(true, true)) return true;

			if (!piece.is_king) return false;

			// King selection
			mv = lio::Vec2i(piece.pos.x + 1, piece.pos.y + (!player.up ? -1 : 1));
			if (PushPossMv(false, false)) return true;

			mv.x -= 2;
			if (PushPossMv(true, false)) return true;
		};
		
		if (!CanMv()) continue;

		// Check still have pieces
		for (auto i = 0; i < 2; ++i)
		{
			if (players[i] == 0)
			{
				players[i] = piece.player_id;
				break;
			}
			else if (players[i] == piece.player_id)
			{
				break;
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