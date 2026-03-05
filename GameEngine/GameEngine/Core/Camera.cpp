#include "Core/Camera.h"

Camera::Camera()
{
    m_position = XMFLOAT3(0.0f, 2.0f, -5.0f);
    m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Camera::SetPosition(float x, float y, float z)
{
    m_position = XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
    m_rotation = XMFLOAT3(pitch, yaw, roll);
}

XMMATRIX Camera::GetViewMatrix()
{
    XMVECTOR pos = XMLoadFloat3(&m_position);

    XMVECTOR forward = XMVectorSet(0, 0, 1, 0);
    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    XMMATRIX rotMatrix =
        XMMatrixRotationRollPitchYaw(
            m_rotation.x,
            m_rotation.y,
            m_rotation.z
        );

    forward = XMVector3TransformCoord(forward, rotMatrix);

    XMVECTOR target = pos + forward;

    return XMMatrixLookAtLH(pos, target, up);
}

XMMATRIX Camera::GetProjectionMatrix(float aspect)
{
    return XMMatrixPerspectiveFovLH(
        XM_PIDIV4,
        aspect,
        0.1f,
        100.0f
    );
}