////////////////////////////////////////////////////////////////////////////////
// Filename: StateMgr.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/StateMgr.h"
#include "../Headers/GameState.h"

StateMgr::~StateMgr()
{
	Release();
}

void StateMgr::ChangeState(GameState* state)
{
	if (!m_vGameStates.empty())
	{
		m_vGameStates.back()->Release();
		delete m_vGameStates.back();
		m_vGameStates.pop_back();
	}

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();
}

void StateMgr::PushState(GameState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();
}

void StateMgr::PopState()
{
	if (!m_vGameStates.empty())
	{
		m_vGameStates.back()->Release();
		delete m_vGameStates.back();
		m_vGameStates.pop_back();
	}

	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void StateMgr::Release()
{
	while (!m_vGameStates.empty())
	{
		m_vGameStates.back()->Release();
		delete m_vGameStates.back();
		m_vGameStates.pop_back();
	}
}

void StateMgr::Pause()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();
}

void StateMgr::Resume()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void StateMgr::ProcessInputs()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->ProcessInputs();
}

void StateMgr::Update(const float& deltaTime)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Update(deltaTime);
}

void StateMgr::Draw()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Draw();
}
