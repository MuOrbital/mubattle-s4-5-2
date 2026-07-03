#pragma once

class CCamera
{
public:
	CCamera();
	virtual ~CCamera();
	void Toggle();
	void Restore();
	void SetIsMove(BOOL IsMove);
	void SetCursorX(LONG CursorX);
	void SetCursorY(LONG CursorY);
	void Zoom(MOUSEHOOKSTRUCTEX* lpMouse);
	void Move(MOUSEHOOKSTRUCTEX* lpMouse);
	void InitCamera();
	void SyncCameraTargets();
	void Update();
	void UpdateSmoothCamera();
	void UpdateSmoothCameraWithMouse();
	void SetZoom(float fStepZoom);
	void SetAngleX(int PitchDirection, float fStepAngle);
	void SetAngleY(int PitchDirection, float fStepAngle, float fStepZ);
	float GetFrustrumCollision();
	bool IsActive();
	void SetCurrentValue();
	void SetAllowed(bool allowed);
	bool IsAllowed() const;

	DWORD LastPress;
	bool m_CameraOnOff;

private:
	bool CheckMouseInPitchZone();

private:
	bool m_Allowed;
	BYTE m_InitCam;
	BYTE m_Enable;
	BOOL m_IsMove;
	int m_vMouseX;
	int m_vMouseY;
	int m_vMouseWheel;
	float m_vCameraFOV;
	float m_vTargetAngleX;
	float m_vTargetAngleY;
	float m_vTargetDistance;
	float m_vFrustrumCollision;
};

extern CCamera gCamera;
