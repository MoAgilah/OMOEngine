////////////////////////////////////////////////////////////////////////////////
// Filename: D3DMgr.h
////////////////////////////////////////////////////////////////////////////////
#ifndef D3DMgr_H
#define D3DMgr_H

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//////////////
// INCLUDES //
//////////////
#include <string>
#include <assert.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DMgr
////////////////////////////////////////////////////////////////////////////////
class D3DMgr
{
public:
	D3DMgr();
	~D3DMgr();

	bool Initialise(const int& screenWidth, const int& screenHeight, const bool& vsync, const HWND& hwnd, const bool& fullscreen,
		const float& screenDepth, const float& screenNear);
	
	void BeginScene(const float& red, const float& green, const float& blue, const float& alpha);
	void EndScene();

	std::pair<int, std::wstring> GetVideoCardInformation();
	ID3D11Device* GetDevice();
	IDXGISwapChain* GetSwapChain();
	ID3D11DeviceContext* GetDeviceContext();
	XMMATRIX* GetProjectionMatrix();
	XMMATRIX* GetWorldMatrix();
	XMMATRIX* GetOrthoMatrix();

	bool OnResize(HWND* hwnd);
	void TurnZBufferOn();
	void TurnZBufferOff();
	void EnableWireframe();
	void DisableWireframe();
private:
	bool CreateDevice(const int& screenWidth, const int& screenHeight, const bool& vsync, const HWND& hwnd, const bool& fullscreen);
	bool SetRenderTargets(const int& screenWidth, const int& screenHeight);
	bool SetRasterStates();
	void SetupViewport(const int& screenWidth, const int& screenHeight, const float& screenDepth, const float& screenNear);


	bool m_VsyncEnabled;
	int m_VideoCardMemory;
	std::wstring m_VideoCardDescription;
	XMMATRIX m_ProjMatrix;
	XMMATRIX m_WorldMatrix;
	XMMATRIX m_OrthoMatrix;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterStateWireframe;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthDisabledStencilState;
};

#endif