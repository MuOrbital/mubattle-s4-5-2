#include "stdafx.h"
#include "Camera3D.h"
#include "ZzzLodTerrain.h"
#include "NewUISystem.h"
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

CCamera gCamera;

static float Clamp(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

CCamera::CCamera()
{
    LastPress = GetTickCount();
    m_CursorX = MouseX;
    m_CursorY = MouseY;
    m_ZoomVelocity = 0.0f;
    m_ZoomSpeed = 0.25f;
    m_TargetYaw = 0.0f;
    m_TargetPitch = 0.0f;
    m_CurrentYaw = 0.0f;
    m_CurrentPitch = 0.0f;
    m_YawVelocity = 0.0f;
    m_PitchVelocity = 0.0f;
    m_RotationSpeed = 0.45f;
    m_PitchSpeed = 0.30f;
    m_IsMiddleMousePressed = false;
    m_LastMouseX = MouseX;
    m_LastMouseY = MouseY;
    m_CameraOnOff = false;
}

CCamera::~CCamera()
{
}

void CCamera::Toggle()
{
    m_CameraOnOff ^= 1;
}

void CCamera::Restore()
{
    CameraZoom = 0.0f;
    CameraAngle[2] = -45.0f;
    AngleY3D = 0.0f;
    AngleZ3D = 0.0f;
    m_TargetYaw = 0.0f;
    m_TargetPitch = 0.0f;
    m_CurrentYaw = 0.0f;
    m_CurrentPitch = 0.0f;
    m_YawVelocity = 0.0f;
    m_PitchVelocity = 0.0f;
    m_ZoomVelocity = 0.0f;
    m_IsMiddleMousePressed = false;
}

void CCamera::Update()
{
    if (SceneFlag != MAIN_SCENE) return;

    static DWORD lastPadPress = 0;

    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    bool padConnected = (XInputGetState(0, &state) == ERROR_SUCCESS);

    if (padConnected && (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB))
    {
        if (GetTickCount() - lastPadPress > 600)
        {
            lastPadPress = GetTickCount();
            Toggle();

            g_pChatListBox->AddText(
                "",
                m_CameraOnOff
                ? "Câmera 3D [ON] - Character Rotation [OFF]"
                : "Câmera 3D [OFF] - Character Rotation [ON]",
                SEASON3B::TYPE_SYSTEM_MESSAGE
            );
        }
    }

    if (HIBYTE(GetAsyncKeyState(VK_F10)) == 128)
    {
        if ((GetTickCount() - LastPress) > 2000)
        {
            LastPress = GetTickCount();
            Toggle();

            g_pChatListBox->AddText(
                "",
                m_CameraOnOff
                ? "Câmera 3D [ON] - Character Rotation [OFF]"
                : "Câmera 3D [OFF] - Character Rotation [ON]",
                SEASON3B::TYPE_SYSTEM_MESSAGE
            );
        }
        return;
    }

    if (!m_CameraOnOff && padConnected)
    {
        const float DEADZONE = 0.20f;
        const float MOUSE_SPEED = 5.0f;

        float LX = state.Gamepad.sThumbLX / 32767.0f;
        float LY = state.Gamepad.sThumbLY / 32767.0f;

        if (fabs(LX) < DEADZONE) LX = 0.0f;
        if (fabs(LY) < DEADZONE) LY = 0.0f;

        if (LX != 0.0f || LY != 0.0f)
        {
            m_CursorX += LX * MOUSE_SPEED;
            m_CursorY -= LY * MOUSE_SPEED;

            if (m_CursorX < 0) m_CursorX = 0;
            if (m_CursorY < 0) m_CursorY = 0;
            if (m_CursorX > GetWindowsX - 1) m_CursorX = GetWindowsX - 1;
            if (m_CursorY > GetWindowsY - 1) m_CursorY = GetWindowsY - 1;

            MouseX = (int)m_CursorX;
            MouseY = (int)m_CursorY;
        }

        return;
    }



    if (!m_CameraOnOff) return;

    static bool isRestoreActive = false;

    if (padConnected && (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB))
    {
        if (GetTickCount() - lastPadPress > 600)
        {
            lastPadPress = GetTickCount();
            isRestoreActive = true;

            g_pChatListBox->AddText(
                "",
                "System [L3] Câmera 3D Restaurada",
                SEASON3B::TYPE_SYSTEM_MESSAGE
            );
        }
    }

    if (padConnected)
    {
        const float DEADZONE = 0.20f;
        const float ANALOG_ZOOM_SPEED = 1.8f;

        float LY = state.Gamepad.sThumbLY / 32767.0f;

        if (fabs(LY) < DEADZONE)
            LY = 0.0f;

        if (LY != 0.0f)
        {
            isRestoreActive = false;

            CameraZoom -= LY * ANALOG_ZOOM_SPEED;
            CameraZoom = Clamp(CameraZoom, -20.0f, 30.0f);
        }
    }


    if (HIBYTE(GetAsyncKeyState(VK_F11)) == 128)
    {
        if ((GetTickCount() - LastPress) > 200)
        {
            LastPress = GetTickCount();
            isRestoreActive = true;

            g_pChatListBox->AddText(
                "",
                "System [F11] Câmera 3D Restaurada",
                SEASON3B::TYPE_SYSTEM_MESSAGE
            );
        }
    }

    if (isRestoreActive)
    {
        float k = 0.13f;

        float deltaYaw = fmodf(-m_TargetYaw, 360.0f);
        if (deltaYaw > 180.0f) deltaYaw -= 360.0f;
        if (deltaYaw < -180.0f) deltaYaw += 360.0f;
        m_TargetYaw += deltaYaw * k;

        float deltaPitch = -m_TargetPitch;
        m_TargetPitch += deltaPitch * k;

        CameraZoom += (0.0f - CameraZoom) * k;

        if (fabs(m_TargetYaw) < 0.1f &&
            fabs(m_TargetPitch) < 0.1f &&
            fabs(CameraZoom) < 0.1f)
        {
            isRestoreActive = false;
            Restore();
        }
    }

    if (HIBYTE(GetAsyncKeyState(VK_MBUTTON)) == 128)
    {
        POINT p;
        GetCursorPos(&p);

        if (!m_IsMiddleMousePressed)
        {
            m_IsMiddleMousePressed = true;
            m_LastMouseX = p.x;
            m_LastMouseY = p.y;
        }
        else
        {
            int dx = p.x - m_LastMouseX;
            int dy = p.y - m_LastMouseY;

            if (dx != 0 || dy != 0)
            {
                isRestoreActive = false;

                m_TargetYaw += (float)dx * m_RotationSpeed;
                m_TargetPitch += (float)dy * m_PitchSpeed;

                m_LastMouseX = p.x;
                m_LastMouseY = p.y;
            }
        }
    }
    else
    {
        m_IsMiddleMousePressed = false;
    }

    if (padConnected)
    {
        const float DEADZONE = 0.20f;

        float RX = state.Gamepad.sThumbRX / 32767.0f;
        float RY = state.Gamepad.sThumbRY / 32767.0f;

        if (fabs(RX) < DEADZONE) RX = 0.0f;
        if (fabs(RY) < DEADZONE) RY = 0.0f;

        if (RX != 0.0f || RY != 0.0f)
        {
            isRestoreActive = false;

            const float ANALOG_YAW_SPEED = 6.0f;
            const float ANALOG_PITCH_SPEED = 5.0f;

            m_TargetYaw += RX * ANALOG_YAW_SPEED;
            m_TargetPitch -= RY * ANALOG_PITCH_SPEED;
        }
    }

    const float stiffnessYaw = 0.22f;
    const float dampingYaw = 0.66f;
    const float stiffnessPitch = 0.28f;
    const float dampingPitch = 0.66f;

    float yawForce = (m_TargetYaw - m_CurrentYaw) * stiffnessYaw;
    m_YawVelocity = (m_YawVelocity + yawForce) * dampingYaw;
    m_CurrentYaw += m_YawVelocity;

    float pitchForce = (m_TargetPitch - m_CurrentPitch) * stiffnessPitch;
    m_PitchVelocity = (m_PitchVelocity + pitchForce) * dampingPitch;
    m_CurrentPitch += m_PitchVelocity;

    m_CurrentPitch = Clamp(m_CurrentPitch, -35.0f, 10.0f);

    CameraAngle[2] = -45.0f + m_CurrentYaw;
    AngleY3D = m_CurrentPitch;
    AngleZ3D = m_CurrentPitch * 18.0f;

    if (MouseWheel > 0) { isRestoreActive = false; m_ZoomVelocity -= m_ZoomSpeed; }
    if (MouseWheel < 0) { isRestoreActive = false; m_ZoomVelocity += m_ZoomSpeed; }
    MouseWheel = 0;

    CameraZoom += m_ZoomVelocity;
    CameraZoom = Clamp(CameraZoom, -30.0f, 25.0f);
    m_ZoomVelocity *= 0.90f;
}