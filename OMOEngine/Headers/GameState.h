////////////////////////////////////////////////////////////////////////////////
// Filename: GameState.h
////////////////////////////////////////////////////////////////////////////////
#ifndef GameState_H
#define GameState_H

/////////////////
// MY INCLUDES //
/////////////////
#include <string>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
// Class name: GameState
////////////////////////////////////////////////////////////////////////////////
class GameState
{
public:
	GameState(std::string name)
		:m_StateName(name)
	{}

	std::string GetName() { return m_StateName; }

	virtual bool Initialise() = 0;
	virtual void Release() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void ProcessInputs() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual void Draw() = 0;

private:
	std::string m_StateName;

};

#endif
