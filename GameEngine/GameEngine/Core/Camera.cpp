#include "Core/Camera.h"

Camera::Camera()
{
    m_position = XMFLOAT3(0.0f, 2.0f, -5.0f);

    m_pitch = 0.0f;
    m_yaw = 0.0f;

    m_moveSpeed = 5.0f;
    m_mouseSensitivity = 0.002f;

    GetCursorPos(&m_lastMousePos);
}

void Camera::SetPosition(float x, float y, float z)
{
    m_position = XMFLOAT3(x, y, z);
}

void Camera::Update(float deltaTime)
{
    float speed = m_moveSpeed * deltaTime;

    POINT mousePos;
    GetCursorPos(&mousePos);

    float dx = (float)(mousePos.x - m_lastMousePos.x);
    float dy = (float)(mousePos.y - m_lastMousePos.y);

    m_lastMousePos = mousePos;

    m_yaw += dx * m_mouseSensitivity;
    m_pitch += dy * m_mouseSensitivity;

    // clamp pitch so camera doesn't flip
    if (m_pitch > XM_PIDIV2 * 0.99f)
        m_pitch = XM_PIDIV2 * 0.99f;

    if (m_pitch < -XM_PIDIV2 * 0.99f)
        m_pitch = -XM_PIDIV2 * 0.99f;

    XMVECTOR forward = XMVectorSet(
        cosf(m_pitch) * sinf(m_yaw),
        sinf(m_pitch),
        cosf(m_pitch) * cosf(m_yaw),
        0
    );

    XMVECTOR right = XMVector3Cross(
        XMVectorSet(0, 1, 0, 0),
        forward
    );

    XMVECTOR pos = XMLoadFloat3(&m_position);

    if (GetAsyncKeyState('W') & 0x8000)
        pos += forward * speed;

    if (GetAsyncKeyState('S') & 0x8000)
        pos -= forward * speed;

    if (GetAsyncKeyState('A') & 0x8000)
        pos -= right * speed;

    if (GetAsyncKeyState('D') & 0x8000)
        pos += right * speed;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        pos += XMVectorSet(0, 1, 0, 0) * speed;

    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
        pos -= XMVectorSet(0, 1, 0, 0) * speed;

    XMStoreFloat3(&m_position, pos);
}

XMMATRIX Camera::GetViewMatrix()
{
    XMVECTOR pos = XMLoadFloat3(&m_position);

    XMVECTOR forward = XMVectorSet(
        cosf(m_pitch) * sinf(m_yaw),
        sinf(m_pitch),
        cosf(m_pitch) * cosf(m_yaw),
        0
    );

    XMVECTOR target = pos + forward;

    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

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