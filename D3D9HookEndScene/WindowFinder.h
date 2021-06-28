#pragma once

#include <Windows.h>

// Finds window by given window name
BOOL GetWindowByName(const char* windowName, HWND &hWindow);

// Checks if window is in fullscreen
BOOL CheckWindowMode(HWND window);