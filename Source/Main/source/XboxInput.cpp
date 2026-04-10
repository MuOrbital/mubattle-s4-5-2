#include "stdafx.h"
#include "XboxInput.h"
#include <windows.h>
#include <Xinput.h>

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef short SHORT;
typedef unsigned char BYTE;

struct XINPUT_GAMEPAD_MIN
{
    WORD  wButtons;
    BYTE  bLeftTrigger;
    BYTE  bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
};

struct XINPUT_STATE_MIN
{
    DWORD dwPacketNumber;
    XINPUT_GAMEPAD_MIN Gamepad;
};

typedef DWORD(WINAPI* XInputGetState_t)(DWORD, XINPUT_STATE_MIN*);

static HMODULE hXInput = NULL;
static XInputGetState_t pXInputGetState = NULL;

static XINPUT_STATE_MIN g_State{};
static XINPUT_STATE_MIN g_Prev{};

static const BYTE TRIGGER_THRESHOLD = 30;

static bool LoadXInput()
{
    if (pXInputGetState)
        return true;

    hXInput = LoadLibraryA("xinput1_4.dll");
    if (!hXInput) hXInput = LoadLibraryA("xinput9_1_0.dll");
    if (!hXInput) hXInput = LoadLibraryA("xinput1_3.dll");

    if (!hXInput)
        return false;

    pXInputGetState = (XInputGetState_t)GetProcAddress(hXInput, "XInputGetState");
    return pXInputGetState != NULL;
}

void XboxInput::Update()
{
    if (!LoadXInput())
        return;

    g_Prev = g_State;
    ZeroMemory(&g_State, sizeof(g_State));

    pXInputGetState(0, &g_State);
}

bool XboxInput::IsPressA()
{
    return (g_State.Gamepad.wButtons & 0x1000) &&
        !(g_Prev.Gamepad.wButtons & 0x1000);
}

bool XboxInput::IsPressX()
{
    return (g_State.Gamepad.wButtons & 0x4000) &&
        !(g_Prev.Gamepad.wButtons & 0x4000);
}

bool XboxInput::IsPressLB()
{
    return (g_State.Gamepad.wButtons & 0x0100) &&
        !(g_Prev.Gamepad.wButtons & 0x0100);
}

bool XboxInput::IsPressRB()
{
    return (g_State.Gamepad.wButtons & 0x0200) &&
        !(g_Prev.Gamepad.wButtons & 0x0200);
}

bool XboxInput::IsPressRT()
{
    return (g_State.Gamepad.bRightTrigger > TRIGGER_THRESHOLD) &&
        (g_Prev.Gamepad.bRightTrigger <= TRIGGER_THRESHOLD);
}

bool XboxInput::IsDownA()
{
    return (g_State.Gamepad.wButtons & 0x1000) != 0;
}

bool XboxInput::IsDownX()
{
    return (g_State.Gamepad.wButtons & 0x4000) != 0;
}

bool XboxInput::IsDownLT()
{
    return g_State.Gamepad.bLeftTrigger > TRIGGER_THRESHOLD;
}

bool XboxInput::IsReleaseA()
{
    return !(g_State.Gamepad.wButtons & 0x1000) &&
        (g_Prev.Gamepad.wButtons & 0x1000);
}

bool XboxInput::IsReleaseX()
{
    return !(g_State.Gamepad.wButtons & 0x4000) &&
        (g_Prev.Gamepad.wButtons & 0x4000);
}

bool XboxInput::IsPressDPadUp()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) &&
        !(g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
}

bool XboxInput::IsDownDPadUp()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
}

bool XboxInput::IsReleaseDPadUp()
{
    return !(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) &&
        (g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
}

bool XboxInput::IsPressDPadDown()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) &&
        !(g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
}

bool XboxInput::IsDownDPadDown()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
}

bool XboxInput::IsReleaseDPadDown()
{
    return !(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) &&
        (g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
}

bool XboxInput::IsPressDPadLeft()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) &&
        !(g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
}

bool XboxInput::IsDownDPadLeft()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
}

bool XboxInput::IsReleaseDPadLeft()
{
    return !(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) &&
        (g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
}

bool XboxInput::IsPressDPadRight()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) &&
        !(g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
}

bool XboxInput::IsDownDPadRight()
{
    return (g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
}

bool XboxInput::IsReleaseDPadRight()
{
    return !(g_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) &&
        (g_Prev.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
}
