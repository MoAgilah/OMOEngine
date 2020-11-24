#include "../Headers/AnimatedSprite.h"
#include <cmath>

AnimatedSprite::AnimatedSprite(D2DMgr* d2d, LPCWSTR imageFile, const char* animFile, D2D1_POINT_2F pos, const unsigned int& initAnim, const float& animFPS)
	:Sprite(d2d, imageFile, pos), m_PlayedOnce(false), m_CurrAnim(initAnim), m_CurrFrame(0), m_AnimFPS(animFPS), m_FrameTime(0.f)
{
	if(!LoadAnimations(animFile))
		MessageBox(NULL, L"Critical Error: Failed to load animations", L"Error Msg", MB_OK);
}

void AnimatedSprite::Update(const float& dt)
{
	UpdateAnimation(dt);
}

void AnimatedSprite::Draw(D2DMgr* d2d, const float&  opacity, const D2D1_BITMAP_INTERPOLATION_MODE& interPol)
{
	AnimCycleData cycleData = m_AnimData[m_CurrAnim];
	
	// compute the destination rectangle, make sure to put the rotation centre at the desired position
	D2D1_RECT_F destRect = { m_Pos.x - (cycleData.m_Width * cycleData.m_RotCntX),				// upper left x: x + width * rotationCenterX
							 m_Pos.y - (cycleData.m_Height * cycleData.m_RotCntY),				// upper left y: y + height * rotationCenterY
							 m_Pos.x + (cycleData.m_Width * (1.0f - cycleData.m_RotCntX)),		// lower right x: x + width * (1-rotationCenterX)
							 m_Pos.y + (cycleData.m_Height * (1.0f - cycleData.m_RotCntY)) };	// lower right y: y + height * (1-rotationCenterY)

	// compute the coordinates of the upper left corner of the source rectangle
	// upper left x of the i-th sprite: border padding plus i-times the combined width of each image and padding between images
	// upper left y of the i-th sprite: border padding plus the combined heights of each image and padding between images in the previous cycles
	float startX = m_CurrFrame * (cycleData.m_Width + cycleData.m_PadWidth) + cycleData.m_BorderWidth;
	float startY = 0;
	
	for (unsigned int i = 0; i < m_CurrAnim; i++)
		startY += m_AnimData[i].m_Height + m_AnimData[i].m_PadHeight;
	startY += m_AnimData[0].m_BorderHeight;

	D2D1_RECT_F sourceRect = { startX, startY, startX + cycleData.m_Width, startY + cycleData.m_Height };
	Sprite::Draw(d2d, &destRect, &sourceRect, opacity, interPol);
}

void AnimatedSprite::UpdateAnimation(const float& deltaTime)
{
	if (!m_PlayedOnce)
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
				if (!m_AnimData[m_CurrAnim].m_IsLooping)	m_PlayedOnce = true;
				m_CurrFrame = m_CurrFrame % m_AnimData[m_CurrAnim].m_NumFrames;
			}
		}

		m_FrameTime = std::fmod(m_FrameTime, 1.f / (double)m_AnimFPS);
	}
}

void AnimatedSprite::ChangeAnimation(const unsigned int& animID)
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

bool AnimatedSprite::LoadAnimations(const char* filename)
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