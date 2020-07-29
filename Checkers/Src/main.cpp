#include <iostream>
#include <chrono>
#include <thread>

#include "Game.h"

Game game;

int main(int argc, char** argv)
{
	using clock = std::chrono::high_resolution_clock;
	DeltaTime detla_time = 0u;

	game.Init(80, 48);

	auto start_time = clock::now();
	while (!game.HasFinished())
	{
		auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start_time).count();
		start_time = clock::now();

		game.Update(delta_time);

		game.Draw();
		
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start_time);
		if (duration.count() < 1e6 / 60)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(15300) - duration);
		}
	}

	int a;
	std::cin >> a;

	return 0;
}