#include "Renderer/Renderer.h"
#pragma comment(lib, "d3d11.lib")

Renderer::Renderer()
    : m_device(nullptr),
    m_context(nullptr),
    m_swapChain(nullptr),
    m_renderTargetView(nullptr)
{
}

Renderer::~Renderer() {}

bool Renderer::Initialize(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = 1280;
    scd.BufferDesc.Height = 720;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_context
    );

    if (FAILED(hr)) return false;

    ID3D11Texture2D* backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
    backBuffer->Release();

    m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

    return true;
}

void Renderer::Render()
{
    float clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    m_swapChain->Present(1, 0);
}

void Renderer::Shutdown()
{
    if (m_renderTargetView) m_renderTargetView->Release();
    if (m_swapChain) m_swapChain->Release();
    if (m_context) m_context->Release();
    if (m_device) m_device->Release();
}