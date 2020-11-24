////////////////////////////////////////////////////////////////////////////////
// Filename: D2DMgr.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../Headers/D2DMgr.h"

D2DMgr::D2DMgr()
	:m_pWriteFactory(nullptr), m_pFactory(nullptr), m_pDevice(nullptr), m_pDeviceContext(nullptr), m_D3Dptr(nullptr)
{
	if (FAILED(CoInitialize(NULL))) MessageBox(NULL, L"Critical Error: Failed to CoInitialise", L"Error Msg", MB_OK);
}

D2DMgr::~D2DMgr()
{
	m_D3Dptr = nullptr;
}

bool D2DMgr::Initialise(D3DMgr* d3dptr, HWND hwnd)
{
	m_D3Dptr = d3dptr;

	if (!CreateDevice(hwnd))
	{
		MessageBox(NULL, L"Critical Error: Failed to Create the D2D device", L"Error Msg", MB_OK);
		return false;
	}

	if (!CreateBitmapRenderTarget(hwnd))
	{
		MessageBox(NULL, L"Critical Error: Failed to Create the D2D bitmap render target", L"Error Msg", MB_OK);
		return false;
	}

	return true;
}

IDWriteFactory2* D2DMgr::GetWriteFactory()
{
	return m_pWriteFactory.Get();
}

IWICImagingFactory2* D2DMgr::GetWICImagingFactory()
{
	return m_pWICFactory.Get();
}

ID2D1DeviceContext1* D2DMgr::GetDeviceContext()
{
	return m_pDeviceContext.Get();
}

bool D2DMgr::CreateDevice(HWND hwnd)
{
	if FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_pWriteFactory))
	{
		MessageBox(NULL, L"Critical Error: Failed to create the direct write factory", L"Error Msg", MB_OK);
		return false;
	}

	if FAILED(CoCreateInstance(CLSID_WICImagingFactory2, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory2, &m_pWICFactory))
	{
		MessageBox(NULL, L"Critical Error: Failed to create the WIC imaging factory", L"Error Msg", MB_OK);
		return false;
	}

	// create the Direct2D factory
	D2D1_FACTORY_OPTIONS options;
#ifndef NDEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	if FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory2), &options, &m_pFactory))
	{
		MessageBox(NULL, L"Critical Error: Failed to create the d2d factory", L"Error Msg", MB_OK);
		return false;
	}

	Microsoft::WRL::ComPtr <IDXGIDevice> dxgiDevice;
	if FAILED(m_D3Dptr->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice))
	{
		MessageBox(NULL, L"Critical Error: Failed to get the dxgi device", L"Error Msg", MB_OK);
		return false;
	}

	if FAILED(m_pFactory->CreateDevice(dxgiDevice.Get(), &m_pDevice))
	{
		MessageBox(NULL, L"Critical Error: Failed to create the Direct2D device", L"Error Msg", MB_OK);
		return false;
	}

	if FAILED(m_pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_pDeviceContext))
	{
		MessageBox(NULL, L"Critical Error: Failed to create the Direct2D device context", L"Error Msg", MB_OK);
		return false;
	}

	return true;
}

bool D2DMgr::CreateBitmapRenderTarget(HWND hwnd)
{
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.0f;
	bp.dpiY = 96.0f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// Direct2D needs the DXGI version of the back buffer
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBuffer;
	if FAILED(m_D3Dptr->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), &dxgiBuffer))
	{
		MessageBox(NULL, L"Critical Error: Failed to get the dxgi buffer", L"Error Msg", MB_OK);
		return false;
	}

	// create the bitmap
	Microsoft::WRL::ComPtr <ID2D1Bitmap1> targetBitmap;
	if FAILED(m_pDeviceContext->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, targetBitmap.GetAddressOf()))
	{
		MessageBox(NULL, L"Critical Error: Failed to create the bitmap render target", L"Error Msg", MB_OK);
		return false;
	}

	// set the newly created bitmap as render target
	m_pDeviceContext->SetTarget(targetBitmap.Get());

	return true;
}
