#include "PieceActionSystem.h"

#include <cassert>

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void PieceActionSystem::EarlyUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
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

	for (auto& e : eman.EntityFilter<Pointer>().entities)
	{
		auto& pointer = e->GetComponent<Pointer>();
		auto& player = eman.GetEntity(pointer.player_id).GetComponent<Player>();

		if (!pointer.active) continue;

		// Piece action require return key pressed
		if (!(!pointer.select_key_down && game.InputMan().GetKeyState(InputManager::KEY_RETURN)))
		{
			pointer.select_key_down = game.InputMan().GetKeyState(InputManager::KEY_RETURN);
			continue;
		}

		for (auto& e2 : eman.EntityFilter<Piece>().entities)
		{
			auto& piece = e2->GetComponent<Piece>();

			if (pointer.player_id != piece.player_id) continue;

			if (pointer.pos == piece.pos)
			{
				// Select
				if (!pointer.selected_piece)
				{
					pointer.selected_piece = e2;

					lio::Vec2i mv(piece.pos.x + 1, piece.pos.y + (player.up ? -1 : 1));
					if (IsPossMv(piece, 1, (player.up ? -1 : 1)))
					{
						pointer.possible_moves.push_back(mv);
					}
					else if (IsPossMv(piece, 2, (player.up ? -2 : 2)) && abs(board.board.At(mv.x, mv.y)) != piece.player_id)
					{
						pointer.possible_moves.push_back(mv + lio::Vec2i(1, (player.up ? -1 : 1)));
					}

					mv.x -= 2;
					if (IsPossMv(piece, -1, (player.up ? -1 : 1)))
					{
						pointer.possible_moves.push_back(mv);
					}
					else if (IsPossMv(piece, -2, (player.up ? -2 : 2)) && abs(board.board.At(mv.x, mv.y)) != piece.player_id)
					{
						pointer.possible_moves.push_back(mv + lio::Vec2i(-1, (player.up ? -1 : 1)));
					}

					if (!piece.is_king) break;

					// King selection
					mv = lio::Vec2i(piece.pos.x + 1, piece.pos.y + (!player.up ? -1 : 1));
					if (IsPossMv(piece, 1, (!player.up ? -1 : 1)))
					{
						pointer.possible_moves.push_back(mv);
					}
					else if (IsPossMv(piece, 2, (!player.up ? -2 : 2)) && abs(board.board.At(mv.x, mv.y)) != piece.player_id)
					{
						pointer.possible_moves.push_back(mv + lio::Vec2i(1, (!player.up ? -1 : 1)));
					}

					mv.x -= 2;
					if (IsPossMv(piece, -1, (!player.up ? -1 : 1)))
					{
						pointer.possible_moves.push_back(mv);
					}
					else if (IsPossMv(piece, -2, (!player.up ? -2 : 2)) && abs(board.board.At(mv.x, mv.y)) != piece.player_id)
					{
						pointer.possible_moves.push_back(mv + lio::Vec2i(-1, (!player.up ? -1 : 1)));
					}
				}
				// Deselect
				else if (pointer.selected_piece->id == e2->id)
				{
					pointer.possible_moves.clear();
					pointer.selected_piece = nullptr;
				}

				break;
			}
		}

		// Move piece
		if (!pointer.selected_piece)
		{
			pointer.select_key_down = game.InputMan().GetKeyState(InputManager::KEY_RETURN);
			continue;
		}

		auto& s_piece = pointer.selected_piece->GetComponent<Piece>();

		for (auto& poss_mv : pointer.possible_moves)
		{
			if (poss_mv == pointer.pos)
			{
				assert(pointer.selected_piece && "Do not have selected piece, but have possible moves.");

				// Eat?
				lio::Vec2i dv = poss_mv - s_piece.pos;
				if (dv.SqrMagnitude() > 1)
				{
					auto mid = s_piece.pos + dv / dv.Abs();
					
					for (auto& p : eman.EntityFilter<Piece>().entities)
					{
						if (p->GetComponent<Piece>().pos == mid)
						{
							p->Destroy();
							break;
						}
					}
				}

				// Move
				s_piece.pos = poss_mv;

				// Become king
				if (s_piece.pos.y == (eman.GetEntity(s_piece.player_id).GetComponent<Player>().up ? 0 : board.board.height - 1))
					s_piece.is_king = true;

				// Reset pointer
				pointer.selected_piece = nullptr;
				pointer.possible_moves.clear();

				// Change active pointer
				pointer.active = false;

				for (auto& p : eman.EntityFilter<Pointer>().entities)
				{
					if (pointer.entity != p->id)
					{
						p->GetComponent<Pointer>().active = true;
						break;
					}
				}

				break;
			}
		}

		pointer.select_key_down = game.InputMan().GetKeyState(InputManager::KEY_RETURN);
	}
}
