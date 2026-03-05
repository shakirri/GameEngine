#pragma once
#include <windows.h>

class Window
{
public:
    Window();
    ~Window();

    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    void Shutdown();

    HWND GetHWND() const { return m_hwnd; }

private:
    HWND m_hwnd;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};