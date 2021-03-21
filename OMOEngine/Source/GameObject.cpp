#include "../Headers/GameObject.h"

GameObject::GameObject(float x, float y)
	:m_Position(x,y)
{
}

GameObject::GameObject(XY pos)
	:m_Position(pos)
{
}

GameObject::~GameObject()
{
}

DX2DGameObj::DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, float x, float y)
	:GameObject(XY(x, y))
{
	m_Sprite = std::make_unique<SSprite>(d2d, layer, imageFile, D2D1::Point2F(GetPosition()->GetX(), GetPosition()->GetY()));
}

DX2DGameObj::DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, XY pos)
	: GameObject(pos)
{
	m_Sprite = std::make_unique<SSprite>(d2d, layer, imageFile, D2D1::Point2F(GetPosition()->GetX(),GetPosition()->GetY()));
}

DX2DGameObj::DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, const char* animFile, float x, float y, const unsigned int& activeAnimation, const float& animationFPS)
	: GameObject(XY(x, y))
{
	m_Sprite = std::make_unique<ASprite>(d2d, layer, imageFile, animFile, D2D1::Point2F(x, y), activeAnimation, animationFPS);
}

DX2DGameObj::DX2DGameObj(D2DMgr* d2d, Layers layer, std::wstring imageFile, const char* animFile, XY pos, const unsigned int& activeAnimation, const float& animationFPS)
	: GameObject(pos)
{
	D2D1::Point2F(pos.m_X, pos.m_Y);
	m_Sprite = std::make_unique<ASprite>(d2d, layer, imageFile, animFile, D2D1::Point2F(pos.m_X, pos.m_Y), activeAnimation, animationFPS);
}



DX2DGameObj::~DX2DGameObj()
{
}

void DX2DGameObj::Update(const float& dt)
{
	m_Sprite->SetPosition(GetPosition()->GetXY());
	m_Sprite->Update(dt);
}

void DX2DGameObj::Draw(Renderer* rend)
{
	m_Sprite->Draw(rend->GetD2DMgr());
}
