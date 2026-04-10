#include "stdafx.h"
#include "AutoClick.h"
#include "NewUISystem.h"


bool AutoClickLeft = false;
bool AutoClickRight = false;
HANDLE AutoClickHandle;


void InitAutoClick() // OK
{
    AutoClickHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AutoClickThread, 0, 0, 0);
}


void WINAPI AutoClickThread() // OK
{
    while (true)
    {
        if (SceneFlag == MAIN_SCENE && GetForegroundWindow() == g_hWnd)
        {
            Sleep(10);


            if (AutoClickLeft == true)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                Sleep(10);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            }


            if (AutoClickRight == true)
            {
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                Sleep(10);
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            }
        }
    }
}


void SwitchKeyLeft() // OK
{
    if (SceneFlag == MAIN_SCENE)
    {
        if (AutoClickLeft ^= true)
        {
            g_pChatListBox->AddText("", "System [F7] AutoClick [ON]", SEASON3B::TYPE_SYSTEM_MESSAGE);
        }
        else
        {
            g_pChatListBox->AddText("", "System [F7] AutoClick [OFF]", SEASON3B::TYPE_SYSTEM_MESSAGE);
        }

    }
}


void SwitchKeyRight() // OK
{
    if (SceneFlag == MAIN_SCENE)
    {
        if (AutoClickRight ^= true)
        {
            g_pChatListBox->AddText("", "System [F9] AutoAttack [ON]", SEASON3B::TYPE_SYSTEM_MESSAGE);
        }
        else
        {
            g_pChatListBox->AddText("", "System [F9] AutoAttack [OFF]", SEASON3B::TYPE_SYSTEM_MESSAGE);
        }
    }
}