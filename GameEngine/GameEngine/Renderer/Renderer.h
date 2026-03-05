#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Core/Camera.h"

using namespace DirectX;

class Renderer
{
public:

    Renderer();
    ~Renderer();

    bool Initialize(HWND hwnd);
    void Render();
    void Shutdown();

private:

    struct Vertex
    {
        float x, y, z;
    };

    struct ConstantBuffer
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    IDXGISwapChain* m_swapChain;

    ID3D11RenderTargetView* m_renderTargetView;

    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11Texture2D* m_depthStencilBuffer;

    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_inputLayout;

    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_constantBuffer;

    Camera m_camera;
};