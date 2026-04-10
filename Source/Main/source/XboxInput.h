#pragma once

class XboxInput
{
public:
    static void Update();

    static bool IsPressA();
    static bool IsPressX();
    static bool IsPressLB();
    static bool IsPressRB();
    static bool IsPressRT();

    static bool IsReleaseA();
    static bool IsReleaseX();

    static bool IsDownA();
    static bool IsDownX();
    static bool IsDownLT();

    static bool IsRepeatA() { return IsDownA(); }
    static bool IsRepeatX() { return IsDownX(); }

    static bool IsPressDPadUp();
    static bool IsDownDPadUp();
    static bool IsReleaseDPadUp();

    static bool IsPressDPadDown();
    static bool IsDownDPadDown();
    static bool IsReleaseDPadDown();

    static bool IsPressDPadLeft();
    static bool IsDownDPadLeft();
    static bool IsReleaseDPadLeft();

    static bool IsPressDPadRight();
    static bool IsDownDPadRight();
    static bool IsReleaseDPadRight();
};