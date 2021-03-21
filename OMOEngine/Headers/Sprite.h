////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Sprite_H
#define Sprite_H

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
#include "../Headers/Point.h"

enum Layers { Background, Scenery, Character, UserInterface, All };
////////////////////////////////////////////////////////////////////////////////
// Class name: Sprite ~ base
////////////////////////////////////////////////////////////////////////////////
class Sprite
{
public:
	Sprite(D2DMgr* d2d, Layers layer, std::wstring imageFile, D2D1_POINT_2F pos);
	virtual ~Sprite() {};

	virtual void Update(const float& dt) = 0;
	virtual void Draw(D2DMgr* d2d, const float& opacity = 1.0f,const D2D1_BITMAP_INTERPOLATION_MODE& interPol = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR) = 0;

	unsigned int GetID() { return m_Identifier; }
	std::wstring GetName() { return m_Name; }
	D2D1_POINT_2F GetPosition() { return m_Position; }
	void SetPosition(Point pnt) { m_Position.x = pnt.GetX(); m_Position.y = pnt.GetY(); }
	ID2D1Bitmap1* GetBitmap() { return m_pBitmap.Get(); }
private:
	unsigned int m_Identifier;
	std::wstring m_Name;
	Layers m_Layer;
	D2D1_POINT_2F m_Position;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pBitmap;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: SSprite ~ StaticSprite
////////////////////////////////////////////////////////////////////////////////
class SSprite : public Sprite
{
public:
	SSprite(D2DMgr* d2d, Layers layer, std::wstring imageFile, D2D1_POINT_2F pos = D2D1::Point2F());
	void Update(const float& dt) override;
	void Draw(D2DMgr* d2d, const float& opacity = 1.0f,
		const D2D1_BITMAP_INTERPOLATION_MODE& interPol = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR) override;
};

///////////////////////////////
// Struct name: AnimCycleData 
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
// Class name:  ASprite ~ AnimatedSprite
////////////////////////////////////////////////////////////////////////////////
class ASprite : public Sprite
{
public:
	ASprite(D2DMgr* d2d, Layers layer, std::wstring imageFile, const char* animFile, D2D1_POINT_2F pos = D2D1::Point2F(), const unsigned int& activeAnimation = 0, const float& animationFPS = 24.f);

	void Update(const float& dt);
	void Draw(D2DMgr* d2d, const float& opacity = 1.0f,
		const D2D1_BITMAP_INTERPOLATION_MODE& interPol = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	void ChangeAnimation(const unsigned int& animID);
	bool HasPlayed() { return m_Played; }
private:
	bool LoadAnimations(const char* filename);
	void UpdateAnimation(const float& deltaTime);

	bool m_Played;
	unsigned int m_CurrAnim;
	unsigned int m_CurrFrame;
	float m_AnimFPS;
	float m_FrameTime;
	std::vector<AnimCycleData> m_AnimData;
};

#endif // !Sprite_H



