#include "Game.h"

Game::Game()
{
	finished = false;
	m_entity_man = lecs::EntityManager();
}

bool Game::HasFinished()
{
	return finished;
}

void Game::Init(int width, int height, int size, short background_col)
{
	m_tcs.Init(width, height, size, "Checkers", background_col);
}

DeltaTime delta_time;

void Game::Update(DeltaTime dt)
{
	delta_time = dt;
}

void Game::Draw()
{
	m_tcs.Clear();
	m_tcs.DrawString(0, 0, std::to_string(1e6 / delta_time));

	m_tcs.Display();
}
