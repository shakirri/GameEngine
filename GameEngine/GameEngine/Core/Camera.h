#pragma once
#include <DirectXMath.h>
#include <windows.h>

using namespace DirectX;

class Camera
{
public:

    Camera();

    void Update(float deltaTime);

    void SetPosition(float x, float y, float z);

    XMMATRIX GetViewMatrix();
    XMMATRIX GetProjectionMatrix(float aspectRatio);

private:

    XMFLOAT3 m_position;

    float m_pitch;
    float m_yaw;

    float m_moveSpeed;
    float m_mouseSensitivity;

    POINT m_lastMousePos;
};