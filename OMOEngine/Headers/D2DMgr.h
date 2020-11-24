////////////////////////////////////////////////////////////////////////////////
// Filename: D2DMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef D2DMgr_H
#define D2DMgr_H

/////////////
// LINKING //
/////////////
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "Windowscodecs.lib")

//////////////
// INCLUDES //
//////////////
#include <vector>
#include <WTypes.h>
#include <wrl/client.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/D3DMgr.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: D2DMgr
////////////////////////////////////////////////////////////////////////////////
class D2DMgr
{
public:
	D2DMgr();
	~D2DMgr();

	bool Initialise(D3DMgr* d3dptr, HWND hwnd);
	
	IDWriteFactory2* GetWriteFactory();
	IWICImagingFactory2* GetWICImagingFactory();
	ID2D1DeviceContext1* GetDeviceContext();
	
private:
	bool CreateDevice(HWND hwnd);					
	bool CreateBitmapRenderTarget(HWND hwnd);		// creates the bitmap render target, set to be the same as the backbuffer already in use for Direct3D

	D3DMgr* m_D3Dptr;
	Microsoft::WRL::ComPtr<IDWriteFactory2> m_pWriteFactory;		
	Microsoft::WRL::ComPtr<ID2D1Factory2> m_pFactory;				
	Microsoft::WRL::ComPtr<IWICImagingFactory2> m_pWICFactory;		
	Microsoft::WRL::ComPtr<ID2D1Device1> m_pDevice;					
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> m_pDeviceContext;	
};

#endif

