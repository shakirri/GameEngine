#include "Renderer/Renderer.h"

#include <d3dcompiler.h>
#include <windows.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

Renderer::Renderer()
{
    m_device = nullptr;
    m_context = nullptr;
    m_swapChain = nullptr;

    m_renderTargetView = nullptr;

    m_depthStencilView = nullptr;
    m_depthStencilBuffer = nullptr;

    m_vertexShader = nullptr;
    m_pixelShader = nullptr;
    m_inputLayout = nullptr;

    m_vertexBuffer = nullptr;
    m_constantBuffer = nullptr;
}

Renderer::~Renderer() {}

bool Renderer::Initialize(HWND hwnd)
{
    HRESULT hr;

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = 1280;
    scd.BufferDesc.Height = 720;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    hr = D3D11CreateDeviceAndSwapChain(
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

    // Back buffer
    ID3D11Texture2D* backBuffer;

    m_swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&backBuffer
    );

    m_device->CreateRenderTargetView(
        backBuffer,
        nullptr,
        &m_renderTargetView
    );

    backBuffer->Release();

    // Depth buffer
    D3D11_TEXTURE2D_DESC depthDesc = {};

    depthDesc.Width = 1280;
    depthDesc.Height = 720;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    m_device->CreateTexture2D(
        &depthDesc,
        nullptr,
        &m_depthStencilBuffer
    );

    m_device->CreateDepthStencilView(
        m_depthStencilBuffer,
        nullptr,
        &m_depthStencilView
    );

    m_context->OMSetRenderTargets(
        1,
        &m_renderTargetView,
        m_depthStencilView
    );

    // Viewport
    D3D11_VIEWPORT viewport = {};

    viewport.Width = 1280;
    viewport.Height = 720;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;

    m_context->RSSetViewports(1, &viewport);

    // Compile shaders
    ID3DBlob* vsBlob;
    ID3DBlob* psBlob;

    D3DCompileFromFile(
        L"Shaders/SimpleVertexShader.hlsl",
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        &vsBlob,
        nullptr
    );

    D3DCompileFromFile(
        L"Shaders/SimplePixelShader.hlsl",
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        0,
        0,
        &psBlob,
        nullptr
    );

    // Create shaders
    m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader
    );

    m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader
    );

    // Input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,
        D3D11_INPUT_PER_VERTEX_DATA,0}
    };

    m_device->CreateInputLayout(
        layout,
        1,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_inputLayout
    );

    vsBlob->Release();
    psBlob->Release();

    m_context->IASetInputLayout(m_inputLayout);

    // Vertex buffer
    Vertex vertices[] =
    {
        { 0.0f, 0.5f, 0.0f },
        { 0.5f,-0.5f, 0.0f },
        {-0.5f,-0.5f, 0.0f }
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices;

    m_device->CreateBuffer(
        &bd,
        &data,
        &m_vertexBuffer
    );

    // Constant buffer
    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    m_device->CreateBuffer(
        &cbd,
        nullptr,
        &m_constantBuffer
    );

    return true;
}

void Renderer::Render()
{
    float color[4] = { 0.1f,0.1f,0.3f,1 };

    m_context->ClearRenderTargetView(
        m_renderTargetView,
        color
    );

    m_context->ClearDepthStencilView(
        m_depthStencilView,
        D3D11_CLEAR_DEPTH,
        1,
        0
    );

    // Matrices
    ConstantBuffer cb;

    cb.world = XMMatrixRotationY(
        (float)GetTickCount64() * 0.001f
    );

    cb.view = m_camera.GetViewMatrix();

    cb.projection = m_camera.GetProjectionMatrix(
        1280.0f / 720.0f
    );

    cb.world = XMMatrixTranspose(cb.world);
    cb.view = XMMatrixTranspose(cb.view);
    cb.projection = XMMatrixTranspose(cb.projection);

    m_context->UpdateSubresource(
        m_constantBuffer,
        0,
        nullptr,
        &cb,
        0,
        0
    );

    m_context->VSSetConstantBuffers(
        0,
        1,
        &m_constantBuffer
    );

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_context->IASetVertexBuffers(
        0,
        1,
        &m_vertexBuffer,
        &stride,
        &offset
    );

    m_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);

    m_context->Draw(3, 0);

    m_swapChain->Present(1, 0);
}

void Renderer::Shutdown()
{
    if (m_constantBuffer) m_constantBuffer->Release();
    if (m_vertexBuffer) m_vertexBuffer->Release();

    if (m_inputLayout) m_inputLayout->Release();
    if (m_vertexShader) m_vertexShader->Release();
    if (m_pixelShader) m_pixelShader->Release();

    if (m_depthStencilView) m_depthStencilView->Release();
    if (m_depthStencilBuffer) m_depthStencilBuffer->Release();

    if (m_renderTargetView) m_renderTargetView->Release();

    if (m_swapChain) m_swapChain->Release();
    if (m_context) m_context->Release();
    if (m_device) m_device->Release();
}