#include "InputManager.h"

#include <windows.h>

#include "../Game.h"

extern Game game;

InputManager::SafeKey::SafeKey()
	: m_pressed(false)
{
}

InputManager::SafeKey::SafeKey(bool init)
	: m_pressed(init)
{
}

InputManager::SafeKey::SafeKey(const SafeKey& k)
{
	m_pressed.store(k.m_pressed.load());
}

InputManager::SafeKey& InputManager::SafeKey::operator=(const SafeKey& k)
{
	m_pressed.store(k.m_pressed.load());
	return *this;
}

bool InputManager::SafeKey::Load()
{
	return m_pressed.load();
}

void InputManager::SafeKey::Store(bool val)
{
	m_pressed.store(val);
}

InputManager::InputManager()
{
	m_keystates.fill(SafeKey(false));
}

bool InputManager::GetKeyState(InputManager::KB_KEYS key)
{
	return m_keystates.at(key).Load();
}

void InputManager::AsyncKeyThread()
{
	while (!game.HasFinished())
	{
		if (GetAsyncKeyState(VK_LEFT))
			m_keystates.at(KEY_LEFT).Store(true);
		else
			m_keystates.at(KEY_LEFT).Store(false);

		if (GetAsyncKeyState(VK_RIGHT))
			m_keystates.at(KEY_RIGHT).Store(true);
		else
			m_keystates.at(KEY_RIGHT).Store(false);

		if (GetAsyncKeyState(VK_UP))
			m_keystates.at(KEY_UP).Store(true);
		else
			m_keystates.at(KEY_UP).Store(false);

		if (GetAsyncKeyState(VK_DOWN))
			m_keystates.at(KEY_DOWN).Store(true);
		else
			m_keystates.at(KEY_DOWN).Store(false);

		if (GetAsyncKeyState(VK_RETURN))
			m_keystates.at(KEY_RETURN).Store(true);
		else
			m_keystates.at(KEY_RETURN).Store(false);
	}
}

