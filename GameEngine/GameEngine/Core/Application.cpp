#include "Core/Application.h"
#include "Platform/Window.h"
#include "Renderer/Renderer.h"

Application::Application()
    : m_window(nullptr), m_renderer(nullptr)
{
}

Application::~Application()
{
    Shutdown();
}

bool Application::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    m_window = new Window();
    if (!m_window->Initialize(hInstance, nCmdShow))
        return false;

    m_renderer = new Renderer();
    if (!m_renderer->Initialize(m_window->GetHWND()))
        return false;

    return true;
}

void Application::Shutdown()
{
    if (m_renderer)
    {
        m_renderer->Shutdown();
        delete m_renderer;
    }

    if (m_window)
    {
        m_window->Shutdown();
        delete m_window;
    }
}

int Application::Run(HINSTANCE hInstance, int nCmdShow)
{
    if (!Initialize(hInstance, nCmdShow))
        return -1;

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_renderer->Render();
        }
    }

    return 0;
}