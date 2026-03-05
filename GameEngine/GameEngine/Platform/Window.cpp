#include "Platform/Window.h"

Window::Window() : m_hwnd(nullptr) {}
Window::~Window() {}

bool Window::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MyEngineWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"My 3D Engine",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!m_hwnd) return false;

    ShowWindow(m_hwnd, nCmdShow);
    return true;
}

void Window::Shutdown()
{
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}