#include "Game.h"

Game::Game()
{
}

void Game::Init(int width, int height, int size, short background_col)
{
	m_lcs.Init(width, height, size / 2, size, "Checkers");
}

void Game::Update()
{
}

void Game::Draw()
{
	m_lcs.Clear();

	m_lcs.Display();
}
