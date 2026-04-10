#include "stdafx.h"
#include "MoveWindow.h"

cMoveWindow::cMoveWindow()
{
	m_PosTemp.x = 0;
	m_PosTemp.y = 0;
	//
	Width = 0;
	Height = 0;
	//
	IsMove = false;
	//
	m_PosOffset.x = 0;
	m_PosOffset.y = 0;
}

cMoveWindow::~cMoveWindow()
{

}

void cMoveWindow::Create(int x, int y, int width, int height)
{
	m_PosTemp.x = x;
	m_PosTemp.y = y;
	//
	Width = width;
	Height = height;
}

bool cMoveWindow::UpdatePosition(int x, int y, int WindowID)
{
	if (SEASON3B::IsRepeat(VK_LBUTTON) == 1 && SEASON3B::CheckMouseIn(x, y, Width, 30))
	{
		this->IsMove = true;
	}
	else
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON) == 0)
		{
			this->IsMove = false;
			//
			m_PosOffset.x = MouseX - x;
			m_PosOffset.y = MouseY - y;
			//
			m_PosTemp.x = x;
			m_PosTemp.y = y;
		}
	}
	
	if (SEASON3B::CheckMouseIn(m_PosTemp.x, m_PosTemp.y, Width, 30) || this->IsMove)
	{
		if (this->IsMove)
		{
			if (MouseX > 0 && MouseX < GetWindowsX)
			{
				m_PosTemp.x = MouseX - m_PosOffset.x;
	
				if (m_PosTemp.x > GetWindowsX - Width)
				{
					m_PosTemp.x = GetWindowsX - Width;
				}
	
				if (m_PosTemp.x < 0)
				{
					m_PosTemp.x = 0;
				}
			}
	
			if (MouseY > 0 && MouseY < GetWindowsY)
			{
				m_PosTemp.y = MouseY - m_PosOffset.y;
	
				if (m_PosTemp.y > GetWindowsY - Height)
				{
					m_PosTemp.y = GetWindowsY - Height;
				}
	
				if (m_PosTemp.y < 0)
				{
					m_PosTemp.y = 0;
				}
			}
	
			return true;
		}
	}
	else
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON) == 0)
		{
			this->IsMove = false;
		}
	}

	return false;
}