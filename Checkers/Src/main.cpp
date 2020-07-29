#include <iostream>
#include <chrono>
#include <thread>

#include "Game.h"

Game game;

int main(int argc, char** argv)
{
	// Init time
	using clock = std::chrono::high_resolution_clock;
	DeltaTime detla_time = 0u;

	// Init game
	game.Init(160, 96, 8);

	// Init input manager
	std::thread inputman_thread(&InputManager::AsyncKeyThread, &game.InputMan());

	// Start game
	auto start_time = clock::now();
	while (!game.HasFinished())
	{
		// Get delta time
		auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start_time).count();
		start_time = clock::now();

		// Game loop
		game.Update(delta_time);

		game.Draw();
		
		// Delay to lock FPS
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start_time);
		if (duration.count() < 1e6 / 60)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(15300) - duration);
		}
	}

	inputman_thread.join();

	return 0;
}