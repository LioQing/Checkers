#include "PointerSystem.h"

#include "../Game.h"
#include "../Components/Components.h"

extern Game game;

void PointerSystem::Update(lecs::EntityManager* eman, lecs::EventManager* evman, DeltaTime dt)
{
	for (auto& e : eman->EntityFilter<Pointer>().entities)
	{
		auto& board = game.board->GetComponent<Board>();
		auto& pointer = e->GetComponent<Pointer>();

		auto KeyAction = [&](InputManager::KB_KEYS k, Pointer::MOVEMENT mv)
		{
			auto pressed = false;

			if (game.InputMan().GetKeyState(k))
			{
				if (pointer.cd_count.at(mv) == 0)
				{
					switch (mv)
					{
					case Pointer::MV_LEFT:
						if (pointer.pos.x == 0) break;
						--pointer.pos.x;
						break;

					case Pointer::MV_RIGHT:
						if (pointer.pos.x == board.width - 1) break;
						++pointer.pos.x;
						break;

					case Pointer::MV_UP:
						if (pointer.pos.y == 0) break;
						--pointer.pos.y;
						break;

					case Pointer::MV_DOWN:
						if (pointer.pos.y == board.height - 1) break;
						++pointer.pos.y;
						break;
					}
				}

				pressed = true;
			}

			if (pressed && pointer.cd_count.at(mv) < (pointer.cd_stage2.at(mv) ? pointer.key_cd2 : pointer.key_cd))
			{
				pointer.cd_count.at(mv) += dt;
			}
			else if (pressed && pointer.cd_count.at(mv) >= (pointer.cd_stage2.at(mv) ? pointer.key_cd2 : pointer.key_cd))
			{
				pointer.cd_stage2.at(mv) = true;
				pointer.cd_count.at(mv) = 0;
			}
			else if (!pressed)
			{
				pointer.cd_stage2.at(mv) = false;
				if (pointer.cd_count.at(mv) > 0)
					pointer.cd_count.at(mv) = 0;
			}
		};

		KeyAction(InputManager::KEY_LEFT, Pointer::MV_LEFT);
		KeyAction(InputManager::KEY_RIGHT, Pointer::MV_RIGHT);
		KeyAction(InputManager::KEY_UP, Pointer::MV_UP);
		KeyAction(InputManager::KEY_DOWN, Pointer::MV_DOWN);
	}
}

void PointerSystem::Draw(lecs::EntityManager* eman, lio::TConsoleScreen* tcs)
{
	for (auto& e : eman->EntityFilter<Pointer>().entities)
	{
		auto& board = game.board->GetComponent<Board>();
		auto& pointer = e->GetComponent<Pointer>();

		// Get tile checker pattern color
		short col = lio::BG_GREY;
		if ((pointer.pos.x + pointer.pos.y) % 2) col = lio::BG_DARK_GREY;

		for (auto x = 0; x < board.tile_size; ++x)
		{
			for (auto y = 0; y < board.tile_size; ++y)
			{
				if (x != 0 && x != board.tile_size - 1 && y != 0 && y != board.tile_size - 1)
					continue;

				auto tx = board.pos.x + pointer.pos.x * board.tile_size + x;
				auto ty = board.pos.y + pointer.pos.y * board.tile_size + y;

				tcs->Draw(tx, ty, lio::PIXEL_THREEQUARTERS, lio::FG_GREEN + col);
			}
		}
	}
}
