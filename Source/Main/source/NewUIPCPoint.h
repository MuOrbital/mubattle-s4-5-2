// PCRoomPoint.h: interface for the CPCRoomPtSys class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CPcPoints
{
protected:
	bool	m_bPCRoom;
	short	m_nNowPt;
	short	m_nMaxPt;
	bool	m_bPCRoomPtShopMode;
	BYTE	m_byBuyItemPos;
protected:
	CPcPoints();
public:
	virtual ~CPcPoints();
	static CPcPoints& Instance();
	void SetPCRoom(bool bPCRoom = true) { m_bPCRoom = bPCRoom; }
	bool IsPCRoom() { return m_bPCRoom; }
	void SetPoint(short nNowPt, short nMaxPt) { m_nNowPt = nNowPt; m_nMaxPt = nMaxPt; }
	short GetNowPoint() { return m_nNowPt; }
	short GetMaxPoint() { return m_nMaxPt; }
	void SetPCRoomPointShopMode(bool bMode = true)
	{
		m_bPCRoomPtShopMode = bMode;
	}
	bool IsPCRoomPointShopMode() { return m_bPCRoomPtShopMode; }

	void SetBuyItemPos(BYTE	byItemPos) { m_byBuyItemPos = byItemPos; }
	BYTE GetBuyItemPos() { return m_byBuyItemPos; }
};
