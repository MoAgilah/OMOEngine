////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Sprite_H
#define Sprite_H

//////////////
// INCLUDES //
//////////////
#include <wrl/client.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D2DMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Sprite
////////////////////////////////////////////////////////////////////////////////
class Sprite
{
public:
	Sprite(D2DMgr* d2d, LPCWSTR imageFile, D2D1_POINT_2F pos = D2D1::Point2F());

	virtual void Update(const float& dt);
	void Draw(D2DMgr* d2d, D2D1_RECT_F* destRect = nullptr, D2D1_RECT_F* sourceRect = nullptr, const float& opacity = 1.0f, const D2D1_BITMAP_INTERPOLATION_MODE& interPol = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
protected:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pBitmap;
	D2D1_POINT_2F m_Pos;
};


#endif // !Sprite_H



