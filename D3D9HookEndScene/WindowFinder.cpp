#include "WindowFinder.h"

BOOL CALLBACK WindowFinder::EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	auto& data = *reinterpret_cast<handle_data*>(lParam);
	HWND hWindow = hwnd;

	// Get prcoess ID from window
	DWORD processID = 0;
	GetWindowThreadProcessId(hWindow, &processID);

	// If processID is not equal to passed processID, keep looping
	if (data.process_id != processID)
		return TRUE;

	// If they match, return FALSE, and save the window handle
	data.window_handle = hWindow;
	return FALSE;
}

DWORD WindowFinder::GetProcessIdByName(const WCHAR* processName)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		// If the strings match it is FALSE
		if (wcscmp(processName, pe32.szExeFile) == FALSE) {
			return pe32.th32ProcessID;
			break;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
}

HWND WindowFinder::GetWindowByProcessName(const WCHAR* processName)
{
	// Get the process ID
	DWORD processID = GetProcessIdByName(processName);
	if (processID != 0) {
		// Create data object
		handle_data data = { 0 };
		data.process_id = processID;
		data.window_handle = nullptr;

		// Get window from process ID
		if (EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data)) == 0) {
			return data.window_handle;
		}
	}
}

HWND WindowFinder::GetWindowByName(const char* windowName)
{
	HWND hWindow = FindWindowA(0, windowName);

	// If window is found
	if (hWindow != NULL)
		return hWindow;

	else
		return nullptr;
}

BOOL WindowFinder::CheckWindowMode(HWND window)
{
	// Structure containing window width and height
	RECT windowRect;

	// Gets the given window's width and height
	GetClientRect(window, &windowRect);

	/*
		Checks if the window's width and height are equal to the primary
		display's width and height. If they're equal the window must be
		fullscreen.
	*/
	if (windowRect.left == 0
		&& windowRect.top == 0
		&& windowRect.right == GetSystemMetrics(SM_CXSCREEN)
		&& windowRect.bottom == GetSystemMetrics(SM_CYSCREEN))
	{
		// Window is in Fullscreen
		return FALSE;
	}
	else
	{
		// Window is in Windowed
		return TRUE;
	}
}
