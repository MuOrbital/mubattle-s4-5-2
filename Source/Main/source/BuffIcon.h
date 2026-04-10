// BuffIcon.h
#pragma once

namespace SEASON3B
{
    struct BuffInfo
    {
        int Index;
        int Time;
        int MaxTime;
    };

    struct IconBuff
    {
        BuffInfo UserBuff[100];
    };

    extern IconBuff gIconBuff;
}