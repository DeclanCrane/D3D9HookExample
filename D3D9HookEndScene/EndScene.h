#pragma once

// D3D9 Includes
#include <d3d9.h>
#include <d3dx9.h>
// D3D9 Libraries
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "WindowFinder.h"

class D3D9Hook
{
public:
	//D3D9 Device
	IDirect3DDevice9*		pDevice;

	//D3D9 Interface
	IDirect3D9*				pDirect3D9;

	//D3D9 Parameters
	D3DPRESENT_PARAMETERS	d3dparams;

	//D3D9 vTable
	void**					vTable;

	// EndScene
	typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* pDevice);
	endScene				pEndScene;

	// D3D9 Reset
	typedef HRESULT(__stdcall* Reset)(D3DPRESENT_PARAMETERS* pPresentationParameters);
	Reset					pReset;

	// Pointer to function to be called inside detour
	HRESULT(__stdcall *Detour)(IDirect3DDevice9* pDevice);

public:

	D3D9Hook();
	~D3D9Hook();

	// Setup D3D9 Parameters
	void SetupD3D9Params(HWND window, BOOL isWindowed);

	// Create D3D9 Device
	void CreateD3D9Device(HWND hWindow);

	// Grabs vTable and creates a detour to EndScene
	void HookEndScene(HWND hWindow, endScene detourFunction);

	// Cleanup Function
	void CleanD3D9();
};