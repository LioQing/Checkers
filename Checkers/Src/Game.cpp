#include "Game.h"

#include "System/Systems.h"
#include "Components/Components.h"

Game::Game()
{
	finished = false;
}

void Game::Init(int width, int height, int size, short background_col)
{
	m_tcs.Init(width, height, size, "Checkers", background_col);

	m_ecsman.system_manager->AddSystem<BoardSystem>();
	m_ecsman.system_manager->AddSystem<PointerSystem>();
	m_ecsman.system_manager->AddSystem<PieceActionSystem>();

	board = &m_ecsman.entity_manager->AddEntity();
	board->AddComponent<Board>(lio::Vec2i(5, 3), 8, 8, 11);

	lecs::Entity* player = &m_ecsman.entity_manager->AddEntity();
	player->AddComponent<Player>(lio::FG_DARK_YELLOW, lio::FG_YELLOW, false);

	lecs::Entity* pointer = &m_ecsman.entity_manager->AddEntity();
	pointer->AddComponent<Pointer>(lio::Vec2i(5, 5), player->id);
	pointer->GetComponent<Pointer>().active = true;

	m_pieceman.SetBoard(*m_ecsman.entity_manager, player->id);
}

void Game::Update(DeltaTime dt)
{
	m_tcs.Clear();
	m_ecsman.system_manager->EarlyUpdate(dt);
	m_ecsman.UpdateECSManagers(dt);
	m_ecsman.system_manager->LateUpdate(dt);

	m_delta_time = dt;
}

void Game::Draw()
{
	

	m_ecsman.system_manager->GetSystem<BoardSystem>().Draw(*m_ecsman.entity_manager, m_tcs);
	m_ecsman.system_manager->GetSystem<PointerSystem>().Draw(*m_ecsman.entity_manager, m_tcs);

	// Show FPS
	m_tcs.DrawString(0, 0, std::to_string(1e6 / m_delta_time), lio::FG_WHITE);

	m_tcs.Display();
}

InputManager& Game::InputMan()
{
	return m_inputman;
}

bool Game::HasFinished()
{
	return finished.load();
}

lio::TConsoleScreen& Game::TCS()
{
	return m_tcs;
}
