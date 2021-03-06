#pragma once

#include <atomic>
#include <array>

class InputManager
{
public:

	enum KB_KEYS
	{
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_RETURN,

		KEY_SIZE
	};

	InputManager();

	bool GetKeyState(InputManager::KB_KEYS key);
	void AsyncKeyThread();

private:

	class SafeKey
	{
	private:

		std::atomic_bool m_pressed;

	public:

		SafeKey();
		SafeKey(bool init);
		SafeKey(const SafeKey& k);

		SafeKey& operator=(const SafeKey& k);

		bool Load();
		void Store(bool val);
	};

	std::array<SafeKey, KEY_SIZE> m_keystates;
};