#include "Game.h"

#include "System/Systems.h"
#include "Components/Components.h"

Game::Game()
{
	finished = false;
}

void Game::Init(int width, int height, int size, short background_col, bool ten)
{
	m_tcs.Init(width, height, size, "Checkers", background_col);

	m_ecsman.system_manager->AddSystem<BoardSystem>();
	m_ecsman.system_manager->AddSystem<PointerSystem>();
	m_ecsman.system_manager->AddSystem<PieceActionSystem>();

	board = &m_ecsman.entity_manager->AddEntity();
	if (!ten)
		board->AddComponent<Board>(lio::Vec2i(5, 3), 8, 8, 11);
	else
		board->AddComponent<Board>(lio::Vec2i(5, 3), 10, 10, 9);

	lecs::Entity* player1 = &m_ecsman.entity_manager->AddEntity();
	player1->AddComponent<Player>(lio::FG_DARK_YELLOW, lio::FG_YELLOW, false);

	lecs::Entity* player2 = &m_ecsman.entity_manager->AddEntity();
	player2->AddComponent<Player>(lio::FG_DARK_RED, lio::FG_RED, true);

	lecs::Entity* pointer = &m_ecsman.entity_manager->AddEntity();
	pointer->AddComponent<Pointer>(lio::Vec2i(5, 5), player1->id);

	lecs::Entity* pointer2 = &m_ecsman.entity_manager->AddEntity();
	pointer2->AddComponent<Pointer>(lio::Vec2i(5, 5), player2->id);

	pointer2->GetComponent<Pointer>().active = true;

	//lecs::Entity* piece = &m_ecsman.entity_manager->AddEntity();
	//piece->AddComponent<Piece>(lio::Vec2i(3, 6), player2->id);

	//lecs::Entity* piece2 = &m_ecsman.entity_manager->AddEntity();
	//piece2->AddComponent<Piece>(lio::Vec2i(3, 4), player1->id);

	//lecs::Entity* piece3 = &m_ecsman.entity_manager->AddEntity();
	//piece3->AddComponent<Piece>(lio::Vec2i(2, 3), player1->id);

	//lecs::Entity* piece4 = &m_ecsman.entity_manager->AddEntity();
	//piece4->AddComponent<Piece>(lio::Vec2i(5, 4), player1->id);

	//lecs::Entity* piece5 = &m_ecsman.entity_manager->AddEntity();
	//piece5->AddComponent<Piece>(lio::Vec2i(6, 3), player1->id);

	//lecs::Entity* piece6 = &m_ecsman.entity_manager->AddEntity();
	//piece6->AddComponent<Piece>(lio::Vec2i(0, 0), player1->id);

	//lecs::Entity* piece7 = &m_ecsman.entity_manager->AddEntity();
	//piece7->AddComponent<Piece>(lio::Vec2i(7, 0), player2->id);

	m_pieceman.SetBoard(*m_ecsman.entity_manager, player1->id);
	m_pieceman.SetBoard(*m_ecsman.entity_manager, player2->id);
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

void Game::Finish()
{
	finished.store(true);
}