#pragma once

#include <memory>

#include "../Headers/Point.h"
#include "../Headers/Sprite.h"
#include "../Headers/Renderer.h"

class GameObject
{
public:
	GameObject(float x, float y);
	GameObject(XY pos);
	virtual ~GameObject();

	virtual void Update(const float& dt) = 0;
	virtual void Draw(Renderer* rend) = 0;

	Point* GetPosition() { return &m_Position; }
	
private:
	Point m_Position;
	
};

class DX2DGameObj : public GameObject
{
public:
	DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, float x, float y);
	DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, XY pos);
	DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, const char* animFile, float x, float y, const unsigned int& activeAnimation = 0, const float& animationFPS = 24.f);
	DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, const char* animFile, XY pos, const unsigned int& activeAnimation = 0, const float& animationFPS = 24.f);

	~DX2DGameObj();

	void Update(const float& dt);
	void Draw(Renderer * rend);
private:
	std::unique_ptr<Sprite> m_Sprite;
};

