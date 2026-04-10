// NewUIPCPoint.cpp: implementation of the NewUIPCPoint class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NewUIPCPoint.h"
#include "ZzzInterface.h"
#include "UsefulDef.h"
#include "UIManager.h"
#include "NewUISystem.h"

extern int GetScreenWidth();
extern int PartyNumber;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPcPoints::CPcPoints() : m_bPCRoom(false), m_nNowPt(0), m_nMaxPt(0), m_bPCRoomPtShopMode(false), m_byBuyItemPos(0xff)
{

}

CPcPoints::~CPcPoints()
{

}

CPcPoints& CPcPoints::Instance()
{
	static CPcPoints s_PCRoomPtSys;
    return s_PCRoomPtSys;
}