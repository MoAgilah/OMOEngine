////////////////////////////////////////////////////////////////////////////////
// Filename: Framework.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/Framework.h"
#include "../Headers/DebugState.h"

SystemPreferences Framework::m_SystPrefs;

Framework::Framework()
	:m_ApplicationName{}, m_HInstance{}, m_Hwnd{}, m_IsPaused{}, m_IsResizing{}, m_IsMinimised{}, m_IsMaximised{},
	m_FrameCnt{}, m_TimeElapsed{}, m_FrameTime{}, mp_StateMgr(std::make_unique<StateMgr>()), mp_Renderer(std::make_unique<Renderer>())
{
	try
	{
		if (!InitWindows(m_SystPrefs.i_ScreenWidth, m_SystPrefs.i_ScreenHeight)) throw;
	}
	catch (const std::exception&)
	{
		MessageBox(NULL, L"Critical Error: Failed to init Window", L"Error Msg", MB_OK);
	}

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	assert(mp_Renderer.get() != nullptr);

	try
	{
		if (!mp_Renderer->Initialise(&m_Hwnd)) throw;
	}
	catch (const std::exception&)
	{
		MessageBox(NULL, L"Critical Error: Failed to init Renderer Module", L"Error Msg", MB_OK);
	}

	try
	{
		if (!Initialise()) throw;
	}
	catch (const std::exception&)
	{
		MessageBox(NULL, L"Critical Error: Failed to initialise the Framework", L"Error Msg", MB_OK);
	}
}

Framework::~Framework()
{
	ShutdownWindows();
}

bool Framework::Initialise()
{
	mp_StateMgr->ChangeState(new DebugState("Debug", &m_Timer, mp_Renderer.get()));

	return true;
}

void Framework::Run()
{
	float accumalator = 0.f;
	float step = 0.1f;
	bool running = true;
	MSG msg;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	m_Timer.Reset();

	// Loop until there is a quit message from the window or the user.
	while (running)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
			break;
		else
		{
			if (!m_IsPaused)
			{
				m_Timer.Tick();

				accumalator += m_Timer.DeltaTime();
				while (accumalator >= step)
				{
					UpdateScene(step);
					accumalator -= step;
				}
				DrawScene();
			}
			else
				Sleep(1000);
		}
	}
}

LRESULT CALLBACK Framework::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//keyboard messages--
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			if (Input::GetInput()->GetKeyboard()->IsKeysAutoRepeat())
				Input::GetInput()->GetKeyboard()->OnKeyPressed(static_cast<unsigned char>(wparam));
			else
			{
				const bool wasPressed = lparam & 0x40000000;

				if (!wasPressed)
					Input::GetInput()->GetKeyboard()->OnKeyPressed(static_cast<unsigned char>(wparam));
			}
		}
		break;


		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			Input::GetInput()->GetKeyboard()->OnKeyReleased(static_cast<unsigned char>(wparam));
		}
		break;

		case WM_CHAR:
		{
			if (Input::GetInput()->GetKeyboard()->IsCharsAutoRepeat())
				Input::GetInput()->GetKeyboard()->OnChar(static_cast<unsigned char>(wparam));
			else
			{
				const bool wasPressed = lparam & 0x40000000;
				if (!wasPressed)
					Input::GetInput()->GetKeyboard()->OnChar(static_cast<unsigned char>(wparam));
			}
		}
		break;

		//Mouse Messages--
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnMouseMove(x, y);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnLeftPressed(x, y);
		}
		break;

		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnRightPressed(x, y);
		}
		break;


		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnMiddlePressed(x, y);
		}
		break;


		case WM_LBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnLeftReleased(x, y);
		}
		break;


		case WM_RBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnRightReleased(x, y);
		}
		break;


		case WM_MBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			Input::GetInput()->GetMouse()->OnMiddleReleased(x, y);
		}
		break;

		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
				Input::GetInput()->GetMouse()->OnWheelUp(x, y);
			else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
				Input::GetInput()->GetMouse()->OnWheelDown(x, y);
		}
		break;

		case WM_SIZE:
		{
			//save the new client area dimensions
			m_SystPrefs.i_ScreenWidth = LOWORD(lparam);
			m_SystPrefs.i_ScreenHeight = HIWORD(lparam);
			
			if (mp_Renderer->GetD3DMgr()->GetDevice())
			{
				if (wparam == SIZE_MINIMIZED)
				{
					m_IsPaused = true;
					m_IsMinimised = true;
					m_IsMaximised = false;
				}
				else if (wparam == SIZE_MAXIMIZED)
				{
					m_IsPaused = false;
					m_IsMinimised = false;
					m_IsMaximised = true;
					mp_Renderer->GetD3DMgr()->OnResize(&hwnd);
				}
				else if (wparam == SIZE_RESTORED)
				{
					if (m_IsMinimised)
					{
						m_IsPaused = false;
						m_IsMinimised = false;
						mp_Renderer->GetD3DMgr()->OnResize(&hwnd);
					}
					else if (m_IsMaximised)
					{
						m_IsPaused = false;
						m_IsMaximised = false;
						mp_Renderer->GetD3DMgr()->OnResize(&hwnd);
					}
					else if (!m_IsResizing)
					{
						mp_Renderer->GetD3DMgr()->OnResize(&hwnd);
					}
				}
			}
		}
		break;

		//OnResizing--
		case WM_ENTERSIZEMOVE:
		{
			m_IsPaused = true;
			m_IsResizing = true;
			mp_StateMgr->Pause();
		}
		break;

		case WM_EXITSIZEMOVE:
		{
			m_IsPaused = false;
			m_IsResizing = false;
			mp_StateMgr->Resume();
			mp_Renderer->GetD3DMgr()->OnResize(&hwnd);
		}
		break;


		//OnActivate/Deactive--
		case WM_ACTIVATE:
		{
			if (LOWORD(wparam) == WA_INACTIVE)
			{
				m_IsPaused = true;
				mp_StateMgr->Pause();
			}
			else
			{
				m_IsPaused = false;
				mp_StateMgr->Resume();
			}
		}
		break;

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Framework::CalculateFrameStats()
{
	// Code computes the average fps, and the avg. time it takes to render one frame. 
	m_FrameCnt++;
	// Compute averages over one second period.
	if ((m_Timer.TotalTime() - m_TimeElapsed) >= 1.0f)
	{
		float fps = (float)m_FrameCnt; // fps = frameCnt / 1
		m_FrameTime = 1000.0f / fps;

		if (m_SystPrefs.b_ShowFPS)
		{
			std::wostringstream OutFPS;
			OutFPS.precision(6);
			OutFPS << "FPS: " << fps << "\n";
			OutFPS << "mSPF: " << m_FrameTime << "\n"; 
			std::wstring str;
			auto vci = mp_Renderer->GetD3DMgr()->GetVideoCardInformation();
			OutFPS << "VideoCard: \n";
			OutFPS << "Name: " << vci.second << "\n";
			OutFPS << "Mem: " << vci.first << "\n";
			m_fpsStr = OutFPS.str();
		}

		// Reset for next average.
		m_FrameCnt = 0;
		m_TimeElapsed += 1.0f;
	}
}

void Framework::ProcessInput()
{
	if (Input::GetInput()->GetKeyboard()->KeyIsPressed(VK_ESCAPE))
		PostQuitMessage(0);
}

void Framework::UpdateScene(const float& deltatime)
{
	mp_StateMgr->Update(deltatime);

	CalculateFrameStats();
	ProcessInput();
}

void Framework::DrawScene()
{
	mp_StateMgr->Draw();
}


bool Framework::InitWindows(int& screenWidth, int& screenHeight)
{
	// Get an external pointer to this object.	
	gs_pApplicationHandle = this;

	// Get the instance of this application.
	m_HInstance = GetModuleHandle(NULL);

	std::wstring tmp(m_SystPrefs.gs_AppName.begin(), m_SystPrefs.gs_AppName.end());
	// Give the application a name.
	m_ApplicationName = tmp.c_str();

	// Setup the windows class with default settings.
	WNDCLASSEX wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_HInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Critical Error: Failed to register the Window class", L"Error Msg", MB_OK);
		return false;
	}

	int posX{}, posY{};
	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_SystPrefs.b_FullScreen)
	{
		// Determine the resolution of the clients desktop screen.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		DEVMODE dmScreenSettings;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to WxH resolution.
		screenWidth = m_SystPrefs.i_ScreenWidth;
		screenHeight = m_SystPrefs.i_ScreenHeight;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	
	int style = m_SystPrefs.b_Bordered ? WS_OVERLAPPEDWINDOW | WS_VISIBLE : WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	// Create the window with the screen settings and get the handle to it.
	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		style, posX, posY, screenWidth, screenHeight, NULL, NULL, m_HInstance, NULL);

	assert(m_Hwnd != nullptr);

	// Bring the window up on the screen and set it as main focus.
	if (!ShowWindow(m_Hwnd, SW_SHOW)) return false;
	if (!SetForegroundWindow(m_Hwnd)) return false;
	m_Hwnd = SetFocus(m_Hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
	if (!SetCursorPos(posX, posY)) return false;

	return true;
}


void Framework::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_SystPrefs.b_FullScreen)
		ChangeDisplaySettings(NULL, 0);

	// Remove the window.
	DestroyWindow(m_Hwnd);
	m_Hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ApplicationName, m_HInstance);
	m_HInstance = NULL;

	// Release the pointer to this class.
	gs_pApplicationHandle = nullptr;
}

SystemPreferences& Framework::GetSystemPrefs()
{
	return m_SystPrefs;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	break;

	// All other messages pass to the message handler in the system class.
	default:
	{
		return gs_pApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}

	return gs_pApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}