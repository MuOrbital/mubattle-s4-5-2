#pragma once

#include "CriticalSection.h"
#include "QueryAsyncManager.h"
#include "Util.h"

#define MAX_QUEUE_QUERY 5000

class QueryJobs {
public:
	QueryJobs() {

	};
	~QueryJobs() {

	};

	std::string name;
	std::string query;
	int aIndex;
	int getResult;
};

class QueryQueue {
public:
	QueryQueue() {
	};

	virtual ~QueryQueue() {
		this->ClearQueue();
	};

	void ClearQueue() {
		this->m_critical.lock();

		this->m_QueueInfo.swap(std::queue<QueryJobs>());

		this->m_critical.unlock();
	};

	DWORD GetQueueSize() {
		DWORD size = 0;

		this->m_critical.lock();

		size = this->m_QueueInfo.size();

		this->m_critical.unlock();

		return size;
	};

	bool AddToQueue(QueryJobs* lpInfo) {
		bool result = 0;

		this->m_critical.lock();

		if (this->m_QueueInfo.size() < MAX_QUEUE_QUERY)
		{
			this->m_QueueInfo.push((*lpInfo));

			result = 1;
		}

		this->m_critical.unlock();

		return result;
	};

	bool GetFromQueue(QueryJobs* lpInfo) {
		bool result = 0;

		this->m_critical.lock();

		if (this->m_QueueInfo.empty() == 0)
		{
			(*lpInfo) = this->m_QueueInfo.front();

			this->m_QueueInfo.pop();

			result = 1;
		}

		this->m_critical.unlock();

		return 1;
	};
private:
	CCriticalSection m_critical;
	std::queue<QueryJobs> m_QueueInfo;
};

struct STRING_RESULTS {
	void Reset() {
		this->name.clear();
		this->result.clear();
	}
	std::string name;
	std::string result;
};

class QueryJobsResult {
public:
	QueryJobsResult() {
		this->result_count = 0;
	};
	~QueryJobsResult() {

	};

	std::list<STRING_RESULTS> result;
	int result_count;
};

class CQueryAsync {
private:
	HANDLE m_ServerQueueSemaphore;
	HANDLE m_ServerQueueThread;
	
	QueryQueue m_jobs;

public:
	CQueryAsyncManager manager;
	std::map<std::string, QueryJobsResult> m_jobsResult;
	DWORD m_jobsID;

public:
	CQueryAsync()
	{
		this->m_ServerQueueThread = 0;
		this->m_ServerQueueSemaphore = 0;

		this->m_jobsID = 0;
	}

	~CQueryAsync()
	{
		if (this->m_ServerQueueThread != 0)
		{
			TerminateThread(this->m_ServerQueueThread, 0);
			CloseHandle(this->m_ServerQueueThread);
			this->m_ServerQueueThread = 0;
		}

		if (this->m_ServerQueueSemaphore != 0)
		{
			CloseHandle(this->m_ServerQueueSemaphore);
			this->m_ServerQueueSemaphore = 0;
		}

		this->m_jobs.ClearQueue();

		this->m_jobsResult.clear();
	}

	void start() {
		if ((this->m_ServerQueueSemaphore = CreateSemaphore(0, 0, MAX_QUEUE_QUERY, 0)) == 0)
		{
			LogAdd(LOG_RED, "[QueryAsync] CreateSemaphore() failed with error: %d", GetLastError());
			return;
		}

		if ((this->m_ServerQueueThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)this->ServerQueueThread, this, 0, 0)) == 0)
		{
			LogAdd(LOG_RED, "[QueryAsync] CreateThread() failed with error: %d", GetLastError());
			return;
		}
	}

	size_t getSize() {
		return this->m_jobs.GetQueueSize() + this->m_jobsResult.size();
	}

	static DWORD WINAPI ServerQueueThread(CQueryAsync* QueryAsync);

	void Query(std::string name, std::string query, int aIndex, int getResult);

	CCriticalSection m_critical;
};

extern CQueryAsync gQueryAsync;