#include "PieceActionSystem.h"

#include <cassert>

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void PieceActionSystem::EarlyUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
	auto& board = game.board->GetComponent<Board>();

	for (auto& e : eman.EntityFilter<Pointer>().entities)
	{
		auto& pointer = e->GetComponent<Pointer>();
		auto& player = eman.GetEntity(pointer.id).GetComponent<Player>();

		if (pointer.selected_piece)game.TCS().DrawString(200, 2, "HAS");

		// Piece action require return key pressed
		if (!(!pointer.select_key_down && game.InputMan().GetKeyState(InputManager::KEY_RETURN)))
		{
			pointer.select_key_down = game.InputMan().GetKeyState(InputManager::KEY_RETURN);
			continue;
		}

		for (auto& e2 : eman.EntityFilter<Piece>().entities)
		{
			auto& piece = e2->GetComponent<Piece>();

			if (pointer.id != piece.player_id) continue;

			if (pointer.pos == piece.pos)
			{
				// Select
				if (!pointer.selected_piece)
				{
					pointer.selected_piece = e2;

					if (piece.pos.x != board.board.width - 1 && 
						piece.pos.y != (player.up ? 0 : board.board.height - 1) &&
						board.board.At(piece.pos.x + 1, piece.pos.y + (player.up ? -1 : 1)) == 0)
					{
						pointer.possible_moves.push_back(lio::Vec2i(piece.pos.x + 1, piece.pos.y + (player.up ? -1 : 1)));
					}
					if (piece.pos.x !=  0 && 
						piece.pos.y != (player.up ? 0 : board.board.height - 1) &&
						board.board.At(piece.pos.x - 1, piece.pos.y + (player.up ? -1 : 1)) == 0)
					{
						pointer.possible_moves.push_back(lio::Vec2i(piece.pos.x - 1, piece.pos.y + (player.up ? -1 : 1)));
					}

					if (!piece.is_king) break;

					// King selection
					if (piece.pos.x != board.board.width - 1 &&
						piece.pos.y != (!player.up ? 0 : board.board.height - 1) &&
						board.board.At(piece.pos.x + 1, piece.pos.y + (!player.up ? -1 : 1)) == 0)
					{
						pointer.possible_moves.push_back(lio::Vec2i(piece.pos.x + 1, piece.pos.y + (!player.up ? -1 : 1)));
					}
					if (piece.pos.x != 0 &&
						piece.pos.y != (!player.up ? 0 : board.board.height - 1) &&
						board.board.At(piece.pos.x - 1, piece.pos.y + (!player.up ? -1 : 1)) == 0)
					{
						pointer.possible_moves.push_back(lio::Vec2i(piece.pos.x - 1, piece.pos.y + (!player.up ? -1 : 1)));
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

				// Move
				s_piece.pos = poss_mv;

				// Become king
				if (s_piece.pos.y == (eman.GetEntity(s_piece.player_id).GetComponent<Player>().up ? 0 : board.board.height - 1))
					s_piece.is_king = true;

				// Reset pointer
				pointer.selected_piece = nullptr;
				pointer.possible_moves.clear();
				break;
			}
		}

		pointer.select_key_down = game.InputMan().GetKeyState(InputManager::KEY_RETURN);
	}
}
