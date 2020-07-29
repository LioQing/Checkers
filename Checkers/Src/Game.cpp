#include "Game.h"

#include "System/Systems.h"
#include "Components/Components.h"

Game::Game()
{
	finished = false;
	m_ecsman = lecs::ECSManagers();
}

void Game::Init(int width, int height, int size, short background_col)
{
	m_tcs.Init(width, height, size, "Checkers", background_col);

	m_ecsman.system_manager->AddSystem<BoardSystem>();
	m_ecsman.system_manager->AddSystem<PointerSystem>();

	board = &m_ecsman.entity_manager->AddEntity();
	board->AddComponent<Board>(lio::Vec2i(5, 3), 10, 10, 8);

	lecs::Entity* pointer = &m_ecsman.entity_manager->AddEntity();
	pointer->AddComponent<Pointer>(lio::Vec2i(5, 5));
}

DeltaTime delta_time;

void Game::Update(DeltaTime dt)
{
	m_ecsman.UpdateECSManagers(dt);

	delta_time = dt;
}

void Game::Draw()
{
	m_tcs.Clear();

	m_ecsman.system_manager->GetSystem<BoardSystem>().Draw(m_ecsman.entity_manager, &m_tcs);
	m_ecsman.system_manager->GetSystem<PointerSystem>().Draw(m_ecsman.entity_manager, &m_tcs);

	// Show FPS
	m_tcs.DrawString(0, 0, std::to_string(delta_time), lio::FG_WHITE);

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
