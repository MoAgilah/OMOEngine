////////////////////////////////////////////////////////////////////////////////
// Filename: MouseEvent.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/MouseEvent.h"

MouseEvent::MouseEvent()
	: m_MEventType(MEventType::Invalid), m_XPos(0), m_YPos(0)
{}

MouseEvent::MouseEvent(const MEventType& type, const int& x, const int& y)
	:m_MEventType(type), m_XPos(x), m_YPos(y)
{}

MouseEvent::~MouseEvent()
{}

bool MouseEvent::IsValid() const
{
	return m_MEventType != MEventType::Invalid;
}

MouseEvent::MEventType MouseEvent::GetType() const
{
	return m_MEventType;
}

MouseEvent::MousePoint MouseEvent::GetPos() const
{
	return { m_XPos, m_YPos };
}

int MouseEvent::GetPosX() const
{
	return m_XPos;
}

int MouseEvent::GetPosY() const
{
	return m_YPos;
}