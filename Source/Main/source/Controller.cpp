#include "stdafx.h"
#include "Controller.h"
#include "Camera3D.h"
#include "ChaosGenesis.h"
#include "InvasionInfo.h"
#include "Interfaces.h"
// ----------------------------------------------------------------------------------------------
Controller	gController;
// ----------------------------------------------------------------------------------------------

bool Controller::Load()
{
	if( !this->MouseHook )
	{
		this->MouseHook = SetWindowsHookEx(WH_MOUSE, this->MouseProc, this->Instance, GetCurrentThreadId());
		// ----
		if( !this->MouseHook )
		{
			return false;
		}
	}
	if (!this->KeyboardHook)
	{
		this->KeyboardHook = SetWindowsHookEx(WH_KEYBOARD, this->KeyboardProc, this->Instance, GetCurrentThreadId());

		if (!this->KeyboardHook)
		{
			return false;
		}
	}

	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------

LRESULT CALLBACK Controller::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (GetForegroundWindow() != g_hWnd)
	{
		return CallNextHookEx(gController.MouseHook, nCode, wParam, lParam);
	}
	if (nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* Mouse = (MOUSEHOOKSTRUCTEX*)lParam;

		gInvasionInfo.Mouse(Mouse, wParam);

		switch (wParam)
		{
		case WM_MOUSEMOVE:
			gCamera.Move(Mouse);
			break;
		case WM_MBUTTONDOWN:
			gCamera.SetIsMove(1);
			gCamera.SetCursorX(Mouse->pt.x);
			gCamera.SetCursorY(Mouse->pt.y);
			break;
		case WM_MBUTTONUP:
			gCamera.SetIsMove(0);
			break;
		case WM_MOUSEWHEEL:
			gCamera.Zoom(Mouse);
			if (SceneFlag == MAIN_SCENE)
			{
				gInterface.m_Lua.Generic_Call("ScrollMouse", "i>", Mouse->mouseData);
			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		break;
		}
	}

	if (nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* HookStruct = (MOUSEHOOKSTRUCTEX*)lParam;

		switch (wParam)
		{
		case WM_RBUTTONUP:
			if (SceneFlag == MAIN_SCENE)
			{
				gInterface.m_Lua.Generic_Call("InterfaceClickRightEvent", ">");
			}
			break;
		case WM_LBUTTONUP:
			if (SceneFlag == MAIN_SCENE)
			{
				gInterface.m_Lua.Generic_Call("InterfaceClickEvent", ">");
			}
			break;
		default:
			break;
		}
	}

	// ----
	return CallNextHookEx(gController.MouseHook, nCode, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------


LRESULT CALLBACK Controller::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (GetForegroundWindow() != g_hWnd)
	{
		return CallNextHookEx(gController.KeyboardHook, nCode, wParam, lParam);
	}

	if (SceneFlag == MAIN_SCENE)
	{
		if (nCode == HC_ACTION)
		{
			if (((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0)
			{
				gInterface.m_Lua.Generic_Call("MainProcWorldKey", "i>", wParam);
			}
		}

		if ((nCode == HC_ACTION) && (wParam == WM_KEYDOWN))
		{
			KBDLLHOOKSTRUCT Hook = *((KBDLLHOOKSTRUCT*)lParam);

			switch (Hook.vkCode)
			{
			case VK_F10:
				break;
			case VK_F11:
				break;
			case VK_HOME:
				break;
			default:
				break;
			}
		}
	}

	return CallNextHookEx(gController.KeyboardHook, nCode, wParam, lParam);
}
