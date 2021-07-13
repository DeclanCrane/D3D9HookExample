// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

#include "WindowFinder.h"
#include "EndScene.h"
#include "ImGuiMenuD3D9.h"

// Handle to the DLL
static HMODULE DllHandle;

D3D9Hook Hook;
ImGuiMenu Menu;

/*
* To use example in another game, change window name to one
* of another application
*/
HWND hWindow = FindWindowA(0, "Call of Duty®: BlackOps");

HRESULT __stdcall myDetour(IDirect3DDevice9* pDevice)
{
    // Setup only used once
    Menu.SetupImGui(hWindow, pDevice);

    // Draws menu every frame
    Menu.Menu();

    // Call original endScene after detour
    return Hook.pEndScene(pDevice);
}

HRESULT __stdcall myResetDetour(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    // Destory and recreate ImGui Device Objects
    ImGui_ImplDX9_InvalidateDeviceObjects();
    ImGui_ImplDX9_CreateDeviceObjects();

    return Hook.pReset(pDevice, pPresentationParameters);
}

DWORD __stdcall EjectThread(LPVOID lpParameter)
{
    Sleep(100); // Safe sleep
    FreeLibraryAndExitThread(DllHandle, 0);
    return 0;
}

DWORD WINAPI MainThread(HINSTANCE hModule)
{
    //-------------------------------//
    // Example of D3D9 EndScene Hook //
    //-------------------------------//
    Hook.HookEndScene(hWindow, myDetour, myResetDetour);

    // Main loop
    while (true)
    {
        // Eject Button
        if (GetAsyncKeyState(VK_END))
        {
            break;
        }
        // Toggle menu
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            Menu.bShowMenu = !Menu.bShowMenu;
        }
    }

    // Creates ejection thread
    CreateThread(NULL, 0, EjectThread, NULL, 0, NULL);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DllHandle = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
