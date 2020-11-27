////////////////////////////////////////////////////////////////////////////////
// Filename: AnimatedSprite.h
////////////////////////////////////////////////////////////////////////////////
#ifndef AnimatedSprite_H
#define AnimatedSprite_H

//////////////
// INCLUDES //
//////////////
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <wrl/client.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D2DMgr.h"
#include "../Headers/Sprite.h"

///////////////////////////////
// Struc name: AnimCycleData 
///////////////////////////////
struct AnimCycleData {
	std::string m_Name;
	bool m_IsLooping;
	unsigned int m_StartFrame;
	unsigned int m_NumFrames;
	float m_Width,
		m_Height,
		m_RotCntX,
		m_RotCntY,
		m_PadWidth,
		m_PadHeight,
		m_BorderWidth,
		m_BorderHeight;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: AnimatedSprite
////////////////////////////////////////////////////////////////////////////////
class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(D2DMgr* d2d, LPCWSTR imageFile, const char* animFile = nullptr, D2D1_POINT_2F pos = D2D1::Point2F(), const unsigned int& activeAnimation = 0, const float& animationFPS = 24.f);
	
	void Update(const float& dt);
	void Draw(D2DMgr* d2d, const float& opacity = 1.0f, const D2D1_BITMAP_INTERPOLATION_MODE& interPol = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	bool LoadAnimations(const char* filename);
	void ChangeAnimation(const unsigned int& animID);

private:
	void UpdateAnimation(const float& deltaTime);

	std::vector<AnimCycleData> m_AnimData;
	bool m_PlayedOnce;
	unsigned int m_CurrAnim;
	unsigned int m_CurrFrame;
	float m_AnimFPS;
	float m_FrameTime;

};

#endif // !AnimatedSprite_H





