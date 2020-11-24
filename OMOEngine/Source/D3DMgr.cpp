////////////////////////////////////////////////////////////////////////////////
// Filename: D3DMgr.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/D3DMgr.h"
#include "../Headers/Framework.h"

D3DMgr::D3DMgr()
	:m_VsyncEnabled{Framework::GetSystemPrefs().b_VsyncEnabled}, m_VideoCardMemory(NULL), m_VideoCardDescription{},
	m_pSwapChain(nullptr), m_pDevice(nullptr), m_pDeviceContext(nullptr), m_pRenderTargetView(nullptr), m_pDepthStencilBuffer(nullptr),
	m_pDepthStencilState(nullptr), m_pDepthStencilView(nullptr), m_pRasterState(nullptr), m_pRasterStateWireframe(nullptr),
	m_pDepthDisabledStencilState(nullptr), m_ProjMatrix(XMMatrixIdentity()), m_OrthoMatrix(XMMatrixIdentity()), m_WorldMatrix(XMMatrixIdentity())
{}

D3DMgr::~D3DMgr()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, NULL);
}

bool D3DMgr::Initialise(const int& screenWidth, const int& screenHeight, const bool& vsync, const HWND& hwnd, const bool& fullscreen,
	const float& screenDepth, const float& screenNear)
{
	// Store the vsync setting.
	m_VsyncEnabled = vsync;

	if (!CreateDevice(screenWidth, screenHeight, vsync, hwnd, fullscreen))
		return false;

	if (!SetRenderTargets(screenWidth, screenHeight))
		return false;

	if (!SetRasterStates())
		return false;
	
	SetupViewport(screenWidth, screenHeight, screenDepth, screenNear);

    return true;
}

void D3DMgr::BeginScene(const float& red, const float& green, const float& blue, const float& alpha)
{
	float color[4]{red, green, blue, alpha};

	// Clear the back buffer.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
    
	// Clear the depth buffer.
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DMgr::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_VsyncEnabled)
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	else
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
}

bool D3DMgr::OnResize(HWND* hwnd)
{
	m_pDeviceContext->ClearState();
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;

	SystemPreferences sp = Framework::GetSystemPrefs();

	if FAILED(m_pSwapChain->ResizeBuffers(0, sp.i_ScreenWidth, sp.i_ScreenHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0))
		return false;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffer(nullptr);

	if FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer))
		return false;

	if FAILED(m_pDevice->CreateRenderTargetView(backbuffer.Get(), NULL, m_pRenderTargetView.ReleaseAndGetAddressOf()))
		return false;

	// create the depth and stencil buffer
	D3D11_TEXTURE2D_DESC dsd;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> dsBuffer;
	backbuffer->GetDesc(&dsd);

	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if FAILED(m_pDevice->CreateTexture2D(&dsd, NULL, dsBuffer.ReleaseAndGetAddressOf()))
		return false;

	if FAILED(m_pDevice->CreateDepthStencilView(dsBuffer.Get(), NULL, &m_pDepthStencilView))
		return false;

	// activate the depth and stencil buffer
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	SetupViewport(sp.i_ScreenWidth, sp.i_ScreenHeight, sp.f_ScreenDepth, sp.f_ScreenNear);

	return true;
}

void D3DMgr::TurnZBufferOn()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
}

void D3DMgr::TurnZBufferOff()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState.Get(), 1);
}

void D3DMgr::EnableWireframe()
{
	m_pDeviceContext->RSSetState(m_pRasterStateWireframe.Get());
}

void D3DMgr::DisableWireframe()
{
	m_pDeviceContext->RSSetState(m_pRasterState.Get());
}

std::pair<int, std::wstring> D3DMgr::GetVideoCardInformation()
{
	return std::pair<int, std::wstring>(m_VideoCardMemory, m_VideoCardDescription);
}

ID3D11Device* D3DMgr::GetDevice()
{
	return m_pDevice.Get();
}

IDXGISwapChain* D3DMgr::GetSwapChain()
{
	return m_pSwapChain.Get();
}

ID3D11DeviceContext* D3DMgr::GetDeviceContext()
{
	return m_pDeviceContext.Get();
}

XMMATRIX* D3DMgr::GetProjectionMatrix()
{
	return &m_ProjMatrix;
}

XMMATRIX* D3DMgr::GetWorldMatrix()
{
	return &m_WorldMatrix;
}

XMMATRIX* D3DMgr::GetOrthoMatrix()
{
	return &m_OrthoMatrix;
}

bool D3DMgr::CreateDevice(const int& screenWidth, const int& screenHeight, const bool& vsync, const HWND& hwnd, const bool& fullscreen)
{
	// Create a DirectX graphics interface factory.
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))	return false;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	if (FAILED(factory->EnumAdapters(0, &adapter)))	return false;

	// Enumerate the primary adapter output (monitor).
	Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))	return false;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes{};
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		return false;

	//Fill the display mode list structures.
	std::unique_ptr<DXGI_MODE_DESC[]> displayModeList(new DXGI_MODE_DESC[numModes]);
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.get())))
		return false;

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	unsigned int numerator(0), denominator(0);
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (UINT)screenWidth && displayModeList[i].Height == (UINT)screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))	return false;

	// Store the dedicated video card memory in megabytes.
	m_VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	m_VideoCardDescription = adapterDesc.Description;

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = (fullscreen) ? false : true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set the refresh rate of the back buffer.
	if (m_VsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };

	// Create the swap chain, Direct3D device, and Direct3D device context.
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext)))
	{
		return false;
	}

	return true;
}

bool D3DMgr::SetRenderTargets(const int & screenWidth, const int & screenHeight)
{
	// Get the pointer to the back buffer.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferPtr;
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
		return false;

	// Create the render target view with the back buffer pointer.
	if (FAILED(m_pDevice->CreateRenderTargetView(backBufferPtr.Get(), NULL, m_pRenderTargetView.ReleaseAndGetAddressOf())))
		return false;

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	if (FAILED(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer)))	
		return false;
	

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
		return false;

	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_pDepthStencilView)))
		return false;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	return true;
}

bool D3DMgr::SetRasterStates()
{
	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	if (FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState)))
	{
		MessageBox(NULL, L"Critical Error: Failed to Create the default rasterizer state", L"Error Msg", MB_OK);
		return false;
	}

	// Now set the rasterizer state.
	m_pDeviceContext->RSSetState(m_pRasterState.Get());

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

	if FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterStateWireframe))
	{
		MessageBox(NULL, L"Critical Error: Failed to Create the wire frame rasterizer state", L"Error Msg", MB_OK);
		return false;
	}

	return true;
}

void D3DMgr::SetupViewport(const int& screenWidth, const int& screenHeight, const float& screenDepth, const float& screenNear)
{
	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports(1, &viewport);
	
	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_ProjMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_WorldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}
