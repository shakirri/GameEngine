#include "Renderer/Renderer.h"
#pragma comment(lib, "d3d11.lib")

Renderer::Renderer()
    : m_device(nullptr),
    m_context(nullptr),
    m_swapChain(nullptr),
    m_renderTargetView(nullptr),
    m_depthStencilView(nullptr),
    m_depthStencilBuffer(nullptr)
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

    if (FAILED(hr))
        return false;

    // === Back Buffer ===
    ID3D11Texture2D* backBuffer = nullptr;
    hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr))
        return false;

    hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
    backBuffer->Release();
    if (FAILED(hr))
        return false;

    // === Depth Buffer ===
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = 1280;
    depthDesc.Height = 720;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_device->CreateTexture2D(&depthDesc, nullptr, &m_depthStencilBuffer);
    if (FAILED(hr))
        return false;

    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView);
    if (FAILED(hr))
        return false;

    // Bind render targets
    m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // === Viewport ===
    D3D11_VIEWPORT viewport = {};
    viewport.Width = 1280.0f;
    viewport.Height = 720.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_context->RSSetViewports(1, &viewport);

    return true;
}

void Renderer::Render()
{
    float clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };

    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);

    m_context->ClearDepthStencilView(
        m_depthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0
    );

    m_swapChain->Present(1, 0);
}

void Renderer::Shutdown()
{
    if (m_depthStencilView)  m_depthStencilView->Release();
    if (m_depthStencilBuffer) m_depthStencilBuffer->Release();
    if (m_renderTargetView)  m_renderTargetView->Release();
    if (m_swapChain)         m_swapChain->Release();
    if (m_context)           m_context->Release();
    if (m_device)            m_device->Release();
}