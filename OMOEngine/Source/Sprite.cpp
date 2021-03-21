#include "../Headers/Sprite.h"

Sprite::Sprite(D2DMgr* d2d, Layers layer, std::wstring imageFile, D2D1_POINT_2F pos)
	: m_Position(pos),m_Layer(layer),m_Identifier(0)
{
	// create decoder
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	if FAILED(d2d->GetWICImagingFactory()->CreateDecoderFromFilename(imageFile.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, bitmapDecoder.ReleaseAndGetAddressOf()))
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

SSprite::SSprite(D2DMgr* d2d, Layers layer, std::wstring imageFile, D2D1_POINT_2F pos)
	:Sprite(d2d, layer, imageFile, pos) {}

void SSprite::Update(const float& dt)
{}

void SSprite::Draw(D2DMgr* d2d, const float& opacity, const D2D1_BITMAP_INTERPOLATION_MODE& interPol)
{
	auto size = GetBitmap()->GetPixelSize();
	auto pos = GetPosition();
	D2D1_RECT_F rect = { pos.x, pos.y, pos.x + size.width, pos.y + size.height };

	d2d->GetDeviceContext()->DrawBitmap(GetBitmap(), rect, opacity, interPol, nullptr);
}

ASprite::ASprite(D2DMgr* d2d, Layers layer, std::wstring imageFile, const char* animFile, D2D1_POINT_2F pos, const unsigned int& initAnim, const float& animFPS)
	:Sprite(d2d, layer, imageFile, pos), m_Played(false), m_CurrAnim(initAnim), m_CurrFrame(0), m_AnimFPS(animFPS), m_FrameTime(0.f)
{
	if (!LoadAnimations(animFile))
		MessageBox(NULL, L"Critical Error: Failed to load animations", L"Error Msg", MB_OK);
}

void ASprite::Update(const float& dt)
{
	UpdateAnimation(dt);
}

void ASprite::Draw(D2DMgr* d2d, const float& opacity, const D2D1_BITMAP_INTERPOLATION_MODE& interPol)
{
	AnimCycleData cycleData = m_AnimData[m_CurrAnim];
	D2D1_POINT_2F Pos = GetPosition();
	// compute the destination rectangle, make sure to put the rotation centre at the desired position
	D2D1_RECT_F destRect = { Pos.x - (cycleData.m_Width * cycleData.m_RotCntX),				// upper left x: x + width * rotationCenterX
							 Pos.y - (cycleData.m_Height * cycleData.m_RotCntY),				// upper left y: y + height * rotationCenterY
							 Pos.x + (cycleData.m_Width * (1.0f - cycleData.m_RotCntX)),		// lower right x: x + width * (1-rotationCenterX)
							 Pos.y + (cycleData.m_Height * (1.0f - cycleData.m_RotCntY)) };	// lower right y: y + height * (1-rotationCenterY)

	// compute the coordinates of the upper left corner of the source rectangle
	// upper left x of the i-th sprite: border padding plus i-times the combined width of each image and padding between images
	// upper left y of the i-th sprite: border padding plus the combined heights of each image and padding between images in the previous cycles
	float startX = m_CurrFrame * (cycleData.m_Width + cycleData.m_PadWidth) + cycleData.m_BorderWidth;
	float startY = 0;

	for (unsigned int i = 0; i < m_CurrAnim; i++)
		startY += m_AnimData[i].m_Height + m_AnimData[i].m_PadHeight;
	startY += m_AnimData[0].m_BorderHeight;

	D2D1_RECT_F sourceRect = { startX, startY, startX + cycleData.m_Width, startY + cycleData.m_Height };
	d2d->GetDeviceContext()->DrawBitmap(GetBitmap(), destRect, opacity, interPol, sourceRect);
}

void ASprite::UpdateAnimation(const float& deltaTime)
{
	if (!m_Played)
	{
		m_FrameTime += deltaTime;

		//check if it's time to change frame
		if (m_FrameTime > (1.f / (double)m_AnimFPS))
		{
			//increment to the next frame 
			m_CurrFrame += (unsigned int)(m_FrameTime * m_AnimFPS);

			//use the modulo op to prevent skip of last frame
			if (m_CurrFrame >= m_AnimData[m_CurrAnim].m_NumFrames)
			{
				if (!m_AnimData[m_CurrAnim].m_IsLooping)	
					m_Played = true;
				m_CurrFrame = m_CurrFrame % m_AnimData[m_CurrAnim].m_NumFrames;
			}
		}

		m_FrameTime = std::fmod(m_FrameTime, 1.f / (double)m_AnimFPS);
	}
}

void ASprite::ChangeAnimation(const unsigned int& animID)
{
	if (animID > m_AnimData.size())
	{
		m_CurrAnim = m_CurrFrame = 0;
		m_FrameTime = 0.f;
	}
	else
	{
		m_CurrAnim = animID;
		m_CurrFrame = 0;
		m_FrameTime = 0.f;
	}
}

bool ASprite::LoadAnimations(const char* filename)
{
	std::ifstream file(filename, std::ios_base::in);
	std::string str;
	AnimCycleData cycle;

	if (file.is_open())
	{
		while (std::getline(file, str))
		{
			std::stringstream ss(str);

			ss >> cycle.m_Name;
			ss >> cycle.m_IsLooping;
			ss >> cycle.m_StartFrame;
			ss >> cycle.m_NumFrames;
			ss >> cycle.m_Width;
			ss >> cycle.m_Height;
			ss >> cycle.m_RotCntX;
			ss >> cycle.m_RotCntY;
			ss >> cycle.m_PadWidth;
			ss >> cycle.m_PadHeight;
			ss >> cycle.m_BorderWidth;
			ss >> cycle.m_BorderHeight;

			m_AnimData.push_back(cycle);
		}

		file.close();
		return true;
	}

	return false;
}


