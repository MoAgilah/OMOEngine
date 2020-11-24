////////////////////////////////////////////////////////////////////////////////
// Filename: Renderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Renderer.h"
#include "../Headers/Framework.h"

Renderer::Renderer()
	:m_pApplicationHandle(nullptr), m_pD3DMgr(std::make_unique<D3DMgr>()), m_pD2DMgr(std::make_unique<D2DMgr>())
{}

Renderer::~Renderer()
{
	m_pApplicationHandle = nullptr;
}

bool Renderer::Initialise(HWND* hwnd)
{
	m_pApplicationHandle = hwnd;
	assert(m_pD3DMgr.get() != nullptr);
	assert(m_pD2DMgr.get() != nullptr);
	SystemPreferences sp = Framework::GetSystemPrefs();

	try
	{
		if (!m_pD3DMgr->Initialise(sp.i_ScreenWidth, sp.i_ScreenHeight, sp.b_VsyncEnabled, *m_pApplicationHandle, sp.b_FullScreen, sp.f_ScreenDepth, sp.f_ScreenNear))
			throw;
	}
	catch (const std::exception&)
	{
		MessageBox(NULL, L"Critical Error: Failed to init D3DMgr", L"Error Msg", MB_OK);
	}

	try
	{
		if (!m_pD2DMgr->Initialise(m_pD3DMgr.get(), *m_pApplicationHandle)) throw;
	}
	catch (const std::exception&)
	{
		MessageBox(NULL, L"Critical Error: Failed to init D2DMgr", L"Error Msg", MB_OK);
	}

	return true;
}

D3DMgr* Renderer::GetD3DMgr()
{
	return m_pD3DMgr.get();
}

D2DMgr* Renderer::GetD2DMgr()
{
	return m_pD2DMgr.get();
}

