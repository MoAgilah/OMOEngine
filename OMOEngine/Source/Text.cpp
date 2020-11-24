#include "../Headers/Text.h"
#include "../Headers/Framework.h"

Text::Text(D2DMgr* D2D, Font f, D2D1::ColorF colour, DWRITE_TEXT_ALIGNMENT textAlign, DWRITE_PARAGRAPH_ALIGNMENT paraAlign)
{
	// set up text formats
	if FAILED(D2D->GetWriteFactory()->CreateTextFormat(f.font, f.fontColl, f.fontWeight, f.fontStretch, f.fontStyle, f.fontSize, f.localeName, &m_pTextFormat))
		MessageBox(NULL, L"Critical Error: Failed to setup the text formats", L"Error Msg", MB_OK);

	//center align horizontal
	if FAILED(m_pTextFormat->SetTextAlignment(textAlign))
		MessageBox(NULL, L"Critical Error: Failed to set text alignment", L"Error Msg", MB_OK);

	if FAILED(m_pTextFormat->SetParagraphAlignment(paraAlign))
		MessageBox(NULL, L"Critical Error: Failed to set the paragraph alignment", L"Error Msg", MB_OK);

	// create standard brushes
	if FAILED(D2D->GetDeviceContext()->CreateSolidColorBrush(colour, &m_pBrush))
		MessageBox(NULL, L"Critical Error: Failed to create the brush", L"Error Msg", MB_OK);
}

void Text::SetText(D2DMgr* D2D, std::wstring text, float maxWidth, float maxHeight)
{
	if FAILED(D2D->GetWriteFactory()->CreateTextLayout(text.c_str(), (UINT32)text.size(), m_pTextFormat.Get(), maxWidth,
		maxHeight, m_pTextLayout.GetAddressOf()))
			MessageBox(NULL, L"Critical Error: Failed to create the text layout", L"Error Msg", MB_OK);
}

IDWriteTextLayout* Text::GetTextLayout()
{
	return m_pTextLayout.Get();
}

void Text::DrawFormat(D2DMgr* D2D, std::wstring text, D2D1_POINT_2F Ori, D2D_RECT_F* layout, D2D1_DRAW_TEXT_OPTIONS ops, DWRITE_MEASURING_MODE mode )
{
	if (layout == nullptr)
	{	
		D2D_RECT_F layout = D2D1::RectF(Ori.x, Ori.y, (FLOAT)Framework::GetSystemPrefs().i_ScreenWidth, (FLOAT)Framework::GetSystemPrefs().i_ScreenHeight);

		D2D->GetDeviceContext()->DrawTextW(text.c_str(), (UINT32)text.size(), m_pTextFormat.Get(), layout, m_pBrush.Get(), ops, mode);
	}
	else
		D2D->GetDeviceContext()->DrawTextW(text.c_str(), (UINT32)text.size(), m_pTextFormat.Get(), layout, m_pBrush.Get(), ops, mode);
}

void Text::DrawLayout(D2DMgr* D2D, D2D1_POINT_2F Ori, D2D1_DRAW_TEXT_OPTIONS ops)
{
	if (m_pTextLayout)
		D2D->GetDeviceContext()->DrawTextLayout(Ori, m_pTextLayout.Get(), m_pBrush.Get(), ops);
}

