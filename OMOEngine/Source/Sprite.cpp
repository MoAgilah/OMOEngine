#include "../Headers/Sprite.h"

Sprite::Sprite(D2DMgr* d2d, LPCWSTR imageFile, D2D1_POINT_2F pos)
	: m_Pos(pos)
{
	// create decoder
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	if FAILED(d2d->GetWICImagingFactory()->CreateDecoderFromFilename(imageFile, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, bitmapDecoder.ReleaseAndGetAddressOf()))
		MessageBox(NULL, L"Critical Error: Failed to create decoder from filename", L"Error Msg", MB_OK);

	// get the correct frame
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	if FAILED(bitmapDecoder->GetFrame(0, frame.ReleaseAndGetAddressOf()))
		MessageBox(NULL, L"Critical Error: Failed to retrieve frame from bitmap", L"Error Msg", MB_OK);

	// create the format converter
	Microsoft::WRL::ComPtr<IWICFormatConverter> image;
	if FAILED(d2d->GetWICImagingFactory()->CreateFormatConverter(image.ReleaseAndGetAddressOf()))
		MessageBox(NULL, L"Critical Error: Failed to create the format converter", L"Error Msg", MB_OK);

	// initialize the WIC image
	if FAILED(image->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom))
		MessageBox(NULL, L"Critical Error: Failed to initialize the WIC image", L"Error Msg", MB_OK);

	// create the bitmap
	if FAILED(d2d->GetDeviceContext()->CreateBitmapFromWicBitmap(image.Get(), m_pBitmap.ReleaseAndGetAddressOf()))
		MessageBox(NULL, L"Failed to create the bitmap image", L"Error Msg", MB_OK);
}

void Sprite::Update(const float& dt)
{}

void Sprite::Draw(D2DMgr* d2d, D2D1_RECT_F* destRect, D2D1_RECT_F* sourceRect, const float& opacity, const D2D1_BITMAP_INTERPOLATION_MODE& interPol)
{
	if (!destRect)
	{
		D2D1_SIZE_U size = m_pBitmap->GetPixelSize();
		D2D1_RECT_F rect = { m_Pos.x, m_Pos.y, m_Pos.x + size.width, m_Pos.y + size.height };

		d2d->GetDeviceContext()->DrawBitmap(m_pBitmap.Get(), rect, opacity, interPol, sourceRect);
	}
	else
	{
		d2d->GetDeviceContext()->DrawBitmap(m_pBitmap.Get(), destRect, opacity, interPol, sourceRect);
	}
}
