#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

#include <string>
#include <vector>

namespace GUIWindow
{
	inline HINSTANCE hInst;
	inline HWND hWnd;
	inline WNDCLASSEXA wc;

	// Data
	inline ID3D11Device* g_pd3dDevice = NULL;
	inline ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	inline IDXGISwapChain* g_pSwapChain = NULL;
	inline ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Setup();
	void Run();
	void Destroy();

	inline std::string appStatus = "Status: Inactive";
	inline std::string currentMap = "";
	inline std::string selfDirectory;
	inline std::string bo3Directory;
}