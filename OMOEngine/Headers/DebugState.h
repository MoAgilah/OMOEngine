////////////////////////////////////////////////////////////////////////////////
// Filename: DebugState.h
////////////////////////////////////////////////////////////////////////////////
#ifndef DebugState_H
#define DebugState_H

/////////////////
// MY INCLUDES //
/////////////////
#include <string>
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/GameState.h"
#include "../Headers/Timer.h"
#include "../Headers/Renderer.h"

#include "../Headers/Text.h"
#include "../Headers/Sprite.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: DebugState
////////////////////////////////////////////////////////////////////////////////
class DebugState : public GameState
{
public:
	DebugState(std::string name, Timer* timer, Renderer* rend);
	~DebugState();

	bool Initialise();
	void Release();
	void Pause();
	void Resume();
	void ProcessInputs();
	void Update(const float& dt);
	void Draw();

private:
	Timer* m_pTimer;
	Renderer* m_pRenderer;
	std::unique_ptr<Text> mp_Text;
	std::unique_ptr<Sprite> m_pSprite;
	std::unique_ptr<AnimatedSprite> m_pWolf;
};

#endif
