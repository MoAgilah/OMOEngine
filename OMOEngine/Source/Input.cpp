////////////////////////////////////////////////////////////////////////////////
// Filename: Input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Input.h"

std::unique_ptr<Input> Input::s_pInstance = nullptr;

Input::Input()
	:m_pKeyboard(std::make_unique<Keyboard>()), m_pMouse(std::make_unique<Mouse>())
{}

Keyboard* Input::GetKeyboard()
{
	return m_pKeyboard.get();
}

Mouse* Input::GetMouse()
{
	return m_pMouse.get();
}

Input* Input::GetInput()
{
	if (!s_pInstance)
	{
		s_pInstance = std::make_unique<Input>();
	}

	return s_pInstance.get();
}

