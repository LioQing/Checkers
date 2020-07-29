#include "InputManager.h"

#include <windows.h>

#include "../Game.h"

extern Game game;

SafeKey::SafeKey()
	: m_pressed(false)
{
}

SafeKey::SafeKey(bool init)
	: m_pressed(init)
{
}

SafeKey::SafeKey(const SafeKey& k)
{
	m_pressed.store(k.m_pressed.load());
}

SafeKey& SafeKey::operator=(const SafeKey& k)
{
	m_pressed.store(k.m_pressed.load());
	return *this;
}

bool SafeKey::Load()
{
	return m_pressed.load();
}

void SafeKey::Store(bool val)
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
	}
}

