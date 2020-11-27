#include "../Headers/DebugState.h"
#include "../Headers/Framework.h"

DebugState::DebugState(std::string name, Timer* timer, Renderer* rend)
	:GameState(name), m_pTimer(timer), m_pRenderer(rend)
{}

DebugState::~DebugState()
{
	Release();
}

bool DebugState::Initialise()
{
	mp_Text = std::make_unique<Text>(m_pRenderer->GetD2DMgr(), Font(), D2D1::ColorF::Black, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	m_pSprite = std::make_unique<Sprite>(m_pRenderer->GetD2DMgr(), L"Resources/Images/bkgrnd.png");
	m_pWolf = std::make_unique<AnimatedSprite>(m_pRenderer->GetD2DMgr(), L"Resources/Images/wolfAnims.png", "Resources/Data/wolfAnims.txt", D2D1::Point2F(400.f, 300.f), 0, 12.f);

	return true;
}

void DebugState::Release()
{
	if (m_pWolf)
		m_pWolf.reset(nullptr);

	if (m_pSprite)
		m_pSprite.reset(nullptr);

	if (mp_Text)
		mp_Text.reset(nullptr);
}

void DebugState::Pause()
{
	m_pTimer->Stop();
}

void DebugState::Resume()
{
	m_pTimer->Start();
}

void DebugState::ProcessInputs()
{
}

void DebugState::Update(const float& dt)
{
	m_pWolf->Update(dt);

	ProcessInputs();
}

void DebugState::Draw()
{
	m_pRenderer->GetD3DMgr()->BeginScene(0.5f, 0.0f, 0.0f, 1.0f);

	//render 3D stuffs--------------------------------
	//------------------------------------------------

	// Turn off the Z buffer to begin all 2D rendering.
	m_pRenderer->GetD3DMgr()->TurnZBufferOff();

	////render 2D stuffs--------------------------------
	m_pRenderer->GetD2DMgr()->GetDeviceContext()->BeginDraw();

	m_pRenderer->GetD2DMgr()->GetDeviceContext()->Clear();

	m_pSprite->Draw(m_pRenderer->GetD2DMgr());

	SystemPreferences sp = Framework::GetSystemPrefs();

	mp_Text->SetText(m_pRenderer->GetD2DMgr(), L"Hello World", (float)sp.i_ScreenWidth, (float)sp.i_ScreenHeight);
	mp_Text->DrawLayout(m_pRenderer->GetD2DMgr(), D2D1::Point2F(0, 0));

	m_pWolf->Draw(m_pRenderer->GetD2DMgr());

	//------------------------------------------------
	m_pRenderer->GetD2DMgr()->GetDeviceContext()->EndDraw();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pRenderer->GetD3DMgr()->TurnZBufferOn();

	m_pRenderer->GetD3DMgr()->EndScene();
}
