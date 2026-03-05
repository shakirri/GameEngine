#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:

    Camera();

    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);

    XMMATRIX GetViewMatrix();
    XMMATRIX GetProjectionMatrix(float aspectRatio);

private:

    XMFLOAT3 m_position;
    XMFLOAT3 m_rotation;
};