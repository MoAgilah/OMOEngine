#ifndef Text_H
#define Text_H

#include <string>
#include <wrl/client.h>
#include <string>
#include "../Headers/D2DMgr.h"

struct Font
{
	WCHAR const* font = L"Gabriola";
	FLOAT fontSize = 18.f;
	WCHAR const* localeName = L"en-us";
	IDWriteFontCollection* fontColl = NULL;
	DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE fontStretch = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH fontStyle = DWRITE_FONT_STRETCH_NORMAL;
};

class Text
{
public:
	// Font collection (NULL sets it to use the system font collection).
	Text(D2DMgr* D2D, Font f, D2D1::ColorF colour, DWRITE_TEXT_ALIGNMENT textAlign, DWRITE_PARAGRAPH_ALIGNMENT paraAlign);

	void SetText(D2DMgr* D2D, std::wstring string, float width, float height);
	IDWriteTextLayout* GetTextLayout();
	
	void DrawFormat(D2DMgr* D2D, std::wstring string, D2D1_POINT_2F Ori, D2D_RECT_F* layout = nullptr, D2D1_DRAW_TEXT_OPTIONS ops = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE mode = DWRITE_MEASURING_MODE_GDI_NATURAL);
	void DrawLayout(D2DMgr* D2D, D2D1_POINT_2F Ori, D2D1_DRAW_TEXT_OPTIONS ops = D2D1_DRAW_TEXT_OPTIONS_NONE);
private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_pTextLayout;
};

#endif // !Text_H
