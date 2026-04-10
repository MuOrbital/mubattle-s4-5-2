#pragma once

class CCamera
{
public:
    CCamera();
    virtual ~CCamera();
    void Toggle();
    void Restore();
    void Update();
    bool m_CameraOnOff;
private:
    DWORD LastPress;
    int m_CursorX;
    int m_CursorY;
    float m_ZoomVelocity;
    float m_ZoomSpeed;
    float m_TargetYaw;
    float m_TargetPitch;
    float m_CurrentYaw;
    float m_CurrentPitch;
    float m_YawVelocity;
    float m_PitchVelocity;
    float m_RotationSpeed;
    float m_PitchSpeed;
    bool m_IsMiddleMousePressed;
    int m_LastMouseX;
    int m_LastMouseY;
};

extern CCamera gCamera;