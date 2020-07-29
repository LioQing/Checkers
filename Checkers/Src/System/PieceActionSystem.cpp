#include "PieceActionSystem.h"

#include "../Components/Components.h"
#include "../Game.h"

extern Game game;

void PieceActionSystem::EarlyUpdate(lecs::EntityManager& eman, lecs::EventManager& evman, DeltaTime dt)
{
	for (auto& e : eman.EntityFilter<Pointer>().entities)
	{
		auto& pointer = e->GetComponent<Pointer>();

		for (auto& e2 : eman.EntityFilter<Piece>().entities)
		{
			auto& piece = e2->GetComponent<Piece>();

			if (pointer.pos == piece.pos && game.InputMan().GetKeyState(InputManager::KEY_RETURN) && !pointer.select_key_down)
			{
				if (!pointer.selected_piece)
				{
					pointer.selected_piece = e2;
				}
				else if (pointer.selected_piece->id == e2->id)
				{
					pointer.selected_piece = nullptr;
				}
			}
		}

		pointer.select_key_down = game.InputMan().GetKeyState(InputManager::KEY_RETURN);
	}
}
