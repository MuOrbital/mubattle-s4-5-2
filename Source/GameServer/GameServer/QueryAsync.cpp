#include "stdafx.h"
#include "QueryAsync.h"
#include "QueryAsyncManager.h"
#include "Util.h"
#include "LuaGameServer.h"
#include "Log.h"

CQueryAsync gQueryAsync;

DWORD __stdcall CQueryAsync::ServerQueueThread(CQueryAsync* QueryAsync)
{
	while (true)
	{
		if (WaitForSingleObject(QueryAsync->m_ServerQueueSemaphore, INFINITE) == WAIT_FAILED)
		{
			LogAdd(LOG_RED, "[QueryAsync] WaitForSingleObject() failed with error: %d", GetLastError());
			break;
		}

		static QueryJobs QueueInfo;

		if (QueryAsync->m_jobs.GetFromQueue(&QueueInfo) != 0)
		{
			auto result = QueryAsync->manager.ExecQuery(QueueInfo.query);

			if (QueueInfo.getResult == 1) {
				std::vector<std::string> columns = QueryAsync->manager.GetColumnNames();
				std::list<STRING_RESULTS> result;
				while (QueryAsync->manager.Fetch() != SQL_NO_DATA) {
					for (size_t i = 0; i < columns.size(); i++) {
						STRING_RESULTS info;

						info.Reset();

						info.name = columns[i];
						info.result = QueryAsync->manager.GetString(i);

						result.push_back(info);
					}
				}

				QueryAsync->manager.Close();

				char buffer[10];
				sprintf_s(buffer, "%d", QueryAsync->m_jobsID);
				QueryAsync->m_jobsID++;

				std::string identification = QueueInfo.name + std::string(buffer);

				QueryJobsResult jobInfo;

				jobInfo.result = result;

				QueryAsync->m_jobsResult.insert(std::make_pair(identification, jobInfo));

				gLuaGameServer.QueryAsyncProcess(QueueInfo.name, identification, QueueInfo.aIndex);
			}
			else {
				QueryAsync->manager.Close();
			}
		}
	}

	return 0;
}

void CQueryAsync::Query(std::string name, std::string query, int aIndex, int getResult)
{
	static QueryJobs QueueInfo;

	QueueInfo.name = name;

	QueueInfo.query = query;

	QueueInfo.aIndex = aIndex;

	QueueInfo.getResult = getResult;

	if (this->m_jobs.AddToQueue(&QueueInfo) != 0)
	{
		ReleaseSemaphore(this->m_ServerQueueSemaphore, 1, 0);
	}
}