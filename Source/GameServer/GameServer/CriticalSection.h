// CriticalSection.h: interface for the CCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CCriticalSection
{
public:
	CCriticalSection();
	virtual ~CCriticalSection();
	void lock();
	void unlock();
private:
	CRITICAL_SECTION m_critical;
};

class CLocker
{
private:
	CCriticalSection& _critical;
public:
	CLocker(CCriticalSection& critical)
		: _critical(critical)
	{
		_critical.lock();
	}

	~CLocker() {
		_critical.unlock();
	}
};