////////////////////////////////////////////////////////////////////////////////
// Filename: Framework.h
////////////////////////////////////////////////////////////////////////////////
#ifndef Framework_H
#define Framework_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <assert.h>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Timer.h"
#include "../Headers/StateMgr.h"
#include "../Headers/Input.h"
#include "../Headers/Renderer.h"

struct SystemPreferences
{
	std::string gs_AppName = "OMOEngine";
	bool b_FullScreen = false;
	bool b_Bordered = true;
	bool b_VsyncEnabled = true;
	bool b_ShowFPS = true;
	int i_ScreenWidth = 800;
	int i_ScreenHeight = 600;
	float f_ScreenDepth = 1000.f;
	float f_ScreenNear = 0.1f;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Framework
////////////////////////////////////////////////////////////////////////////////
class Framework
{
public:
	Framework();
	~Framework();

	void Run();
	void CalculateFrameStats();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	static SystemPreferences& GetSystemPrefs();

private:
	bool InitWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();
	bool Initialise();
	void ProcessInput();
	void UpdateScene(const float& deltaTime);
	void DrawScene();

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_HInstance;
	HWND m_Hwnd;
	static SystemPreferences m_SystPrefs;

	bool m_IsPaused;
	bool m_IsResizing;
	bool m_IsMinimised;
	bool m_IsMaximised;

	int m_FrameCnt;
	float m_TimeElapsed;
	float m_FrameTime;
	std::wstring m_fpsStr;

	Timer m_Timer;
	std::unique_ptr<StateMgr> mp_StateMgr;
	std::unique_ptr<Renderer> mp_Renderer;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS //
/////////////
static Framework* gs_pApplicationHandle = nullptr;

#endif