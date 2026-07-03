#include "stdafx.h"
#include "Camera3D.h"
#include "NewUISystem.h"
#include "ZzzInterface.h"

CCamera gCamera;

extern float vCameraFOV;
extern float vCameraAngleY;
extern float vTargetDistance;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

static float CameraClampFloat(float Value, float Min, float Max)
{
	if (Value < Min)
	{
		return Min;
	}

	if (Value > Max)
	{
		return Max;
	}

	return Value;
}

CCamera::CCamera()
{
	this->m_Allowed = true;
	this->m_InitCam = 1;
	this->m_Enable = 0;
	this->m_IsMove = 0;
	this->m_vMouseX = -1;
	this->m_vMouseY = -1;
	this->m_vMouseWheel = 0;
	this->m_vCameraFOV = 35.0f;
	this->m_vTargetAngleY = -48.5f;
	this->m_vTargetAngleX = -45.0f;
	this->m_vTargetDistance = 150.0f;
	this->m_vFrustrumCollision = 0.0f;
	this->m_CameraOnOff = false;
	this->LastPress = GetTickCount();
}

CCamera::~CCamera()
{
}

void CCamera::Toggle()
{
	if (SceneFlag != MAIN_SCENE)
	{
		return;
	}

	if (this->m_Allowed == false)
	{
		this->m_Enable = 0;
		this->m_CameraOnOff = false;
		this->InitCamera();
		g_pChatListBox->AddText("", "Camera 3D disponivel apenas para VIP.", SEASON3B::TYPE_SYSTEM_MESSAGE);
		return;
	}

	this->m_Enable ^= 1;
	this->m_CameraOnOff = (this->m_Enable != 0);
	this->m_vMouseWheel = 0;
	MouseWheel = 0;

	g_pChatListBox->AddText(
		"",
		(this->m_Enable != 0) ? "Camera 3D [ON] - Character Rotation [OFF]" : "Camera 3D [OFF] - Character Rotation [ON]",
		SEASON3B::TYPE_SYSTEM_MESSAGE);
}

void CCamera::Restore()
{
	if (SceneFlag != MAIN_SCENE)
	{
		return;
	}

	this->InitCamera();
	this->m_vMouseWheel = 0;
	MouseWheel = 0;
}

void CCamera::SetIsMove(BOOL IsMove)
{
	if (this->m_Enable != 0 && SceneFlag == MAIN_SCENE)
	{
		this->m_IsMove = IsMove;
	}
	else
	{
		this->m_IsMove = 0;
	}
}

void CCamera::SetCursorX(LONG CursorX)
{
	if (this->m_Enable != 0 && SceneFlag == MAIN_SCENE)
	{
		this->m_vMouseX = MouseX;
	}
}

void CCamera::SetCursorY(LONG CursorY)
{
	if (this->m_Enable != 0 && SceneFlag == MAIN_SCENE)
	{
		this->m_vMouseY = MouseY;
	}
}

void CCamera::Zoom(MOUSEHOOKSTRUCTEX* lpMouse)
{
	if (this->m_Enable == 0 || this->m_IsMove != 0 || SceneFlag != MAIN_SCENE)
	{
		return;
	}

	this->m_vMouseWheel = (((int)lpMouse->mouseData) > 0) ? 1 : -1;
	MouseWheel = this->m_vMouseWheel;
}

void CCamera::Move(MOUSEHOOKSTRUCTEX* lpMouse)
{
	if (this->m_Enable == 0 || this->m_IsMove == 0 || SceneFlag != MAIN_SCENE)
	{
		return;
	}
}

void CCamera::InitCamera()
{
	this->m_InitCam = 1;
}

void CCamera::SyncCameraTargets()
{
	if (this->m_InitCam != 0)
	{
		this->m_InitCam = 0;
		this->m_vMouseX = -1;
		this->m_vMouseY = -1;
		this->m_vCameraFOV = 35.0f;
		this->m_vTargetAngleY = -48.5f;
		this->m_vTargetAngleX = -45.0f;
		this->m_vTargetDistance = 150.0f;
		this->m_vFrustrumCollision = 0.0f;
	}
}

void CCamera::Update()
{
	if (SceneFlag != MAIN_SCENE)
	{
		return;
	}

	if (HIBYTE(GetAsyncKeyState(VK_F10)) == 128)
	{
		if ((GetTickCount() - this->LastPress) > 300)
		{
			this->LastPress = GetTickCount();
			this->Toggle();
		}

		return;
	}

	if (HIBYTE(GetAsyncKeyState(VK_F11)) == 128)
	{
		if ((GetTickCount() - this->LastPress) > 200)
		{
			this->LastPress = GetTickCount();
			this->Restore();
			g_pChatListBox->AddText("", "System [F11] Camera 3D Restaurada", SEASON3B::TYPE_SYSTEM_MESSAGE);
		}
	}

}

void CCamera::UpdateSmoothCamera()
{
	if (SceneFlag != MAIN_SCENE)
	{
		return;
	}

	if (this->m_Allowed == false)
	{
		if (this->m_Enable != 0 || this->m_CameraOnOff != false)
		{
			this->Restore();
		}

		return;
	}

	this->SyncCameraTargets();

	if (this->m_Enable != 0)
	{
		if (g_pNewUISystem->CheckMouseUse() == false && MouseOnWindow == false && g_dwMouseUseUIID == 0)
		{
			this->UpdateSmoothCameraWithMouse();
		}
	}

	if (this->m_vTargetAngleX >= 360.0f)
	{
		this->m_vTargetAngleX -= 360.0f;
		CameraAngle[2] -= 360.0f;
	}
	else if (this->m_vTargetAngleX <= -360.0f)
	{
		this->m_vTargetAngleX += 360.0f;
		CameraAngle[2] += 360.0f;
	}

	LInterpolationF(vCameraFOV, vCameraFOV, this->m_vCameraFOV, 0.05f);
	LInterpolationF(vCameraAngleY, vCameraAngleY, this->m_vTargetAngleY, 0.05f);
	LInterpolationF(CameraAngle[2], CameraAngle[2], this->m_vTargetAngleX, 0.05f);
	LInterpolationF(vTargetDistance, vTargetDistance, this->m_vTargetDistance, 0.05f);

	CameraZoom = vCameraFOV - 35.0f;
	AngleY3D = vCameraAngleY + 48.5f;
	AngleZ3D = 150.0f - vTargetDistance;

	if (CameraFOV > 35.0f)
	{
		float Factor = (CameraFOV - 35.0f) / 35.0f;
		Factor = CameraClampFloat(Factor, 0.0f, 1.0f);
		this->m_vFrustrumCollision = -250.0f * Factor;
	}
	else
	{
		this->m_vFrustrumCollision = 0.0f;
	}

	this->SetCurrentValue();

	this->m_vMouseX = MouseX;
	this->m_vMouseY = MouseY;
}

void CCamera::UpdateSmoothCameraWithMouse()
{
	if (this->m_vMouseX == -1)
	{
		this->m_vMouseX = MouseX;
	}

	if (this->m_vMouseY == -1)
	{
		this->m_vMouseY = MouseY;
	}

	int CurrentMouseX = MouseX;
	int CurrentMouseY = MouseY;
	int Yaw = (this->m_vMouseX - CurrentMouseX);
	int Pitch = (CurrentMouseY - this->m_vMouseY);

	if (this->m_IsMove != 0)
	{
		if (Yaw != 0 && this->CheckMouseInPitchZone() == false)
		{
			this->SetAngleX(1, 1.0f * Yaw);
		}

		if (Pitch != 0 && this->CheckMouseInPitchZone() != false)
		{
			this->SetAngleY(1, 1.0f * Pitch, 18.0f * Pitch);
		}
	}

	int Wheel = this->m_vMouseWheel;

	if (Wheel == 0)
	{
		Wheel = MouseWheel;
	}

	if (Wheel != 0)
	{
		this->SetZoom(Wheel * 3.5f);
		this->m_vMouseWheel = 0;
		MouseWheel = 0;
	}
}

void CCamera::SetZoom(float fStepZoom)
{
	float FOV = this->m_vCameraFOV - fStepZoom;

	if (FOV >= 17.5f && FOV <= 70.0f)
	{
		this->m_vCameraFOV = FOV;
	}
}

void CCamera::SetAngleX(int PitchDirection, float fStepAngle)
{
	this->m_vTargetAngleX += PitchDirection * fStepAngle;
}

void CCamera::SetAngleY(int PitchDirection, float fStepAngle, float fStepZ)
{
	float AngleY = this->m_vTargetAngleY + (PitchDirection * fStepAngle);

	if (AngleY >= -93.5f && AngleY <= -45.5f)
	{
		this->m_vTargetAngleY = AngleY;
		this->m_vTargetDistance -= (PitchDirection * fStepZ);
	}

	if (this->m_vTargetDistance < -15.0f)
	{
		this->m_vTargetDistance = -15.0f;
	}

	if (this->m_vTargetDistance > 930.0f)
	{
		this->m_vTargetDistance = 930.0f;
	}
}

float CCamera::GetFrustrumCollision()
{
	if (SceneFlag == MAIN_SCENE)
	{
		return this->m_vFrustrumCollision;
	}

	return 0.0f;
}

bool CCamera::IsActive()
{
	return (this->m_Enable != 0);
}

void CCamera::SetCurrentValue()
{
	float Distance = this->m_vTargetDistance - 150.0f;

	if (Distance < 0.0f)
	{
		Distance = -Distance;
	}

	float requiredViewFar = 2500.0f + (Distance * 3.0f);

	if (CameraViewFar < requiredViewFar)
	{
		CameraViewFar = requiredViewFar;
	}
}

void CCamera::SetAllowed(bool allowed)
{
	this->m_Allowed = allowed;

	if (this->m_Allowed == false)
	{
		this->m_Enable = 0;
		this->m_CameraOnOff = false;
		this->Restore();
	}
}

bool CCamera::IsAllowed() const
{
	return this->m_Allowed;
}

bool CCamera::CheckMouseInPitchZone()
{
	float WindowW = ((g_fScreenRate_x > 0.0f) ? ((float)WindowWidth / g_fScreenRate_x) : 640.0f);
	float WindowH = ((g_fScreenRate_y > 0.0f) ? ((float)WindowHeight / g_fScreenRate_y) : 480.0f);

	if (WindowW <= 0.0f)
	{
		WindowW = 640.0f;
	}

	if (WindowH <= 0.0f)
	{
		WindowH = 480.0f;
	}

	float MidCenter = (WindowW * 0.5f) - 50.0f;

	return (MouseX >= MidCenter && MouseX <= (MidCenter + 100.0f) && MouseY >= 0 && MouseY <= WindowH);
}
