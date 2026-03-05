#pragma once
#include <windows.h>

class Window;
class Renderer;

class Application
{
public:
    Application();
    ~Application();

    int Run(HINSTANCE hInstance, int nCmdShow);

private:
    Window* m_window;
    Renderer* m_renderer;

    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    void Shutdown();
};