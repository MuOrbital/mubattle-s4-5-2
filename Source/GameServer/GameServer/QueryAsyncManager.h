// QueryManager.h: interface for the CQueryAsyncManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include "CriticalSection.h"

#define MAX_COLUMNS 100

class CQueryAsyncManager
{
public:
	CQueryAsyncManager() {
		this->m_SQLEnvironment = SQL_NULL_HANDLE;
		this->m_SQLConnection = SQL_NULL_HANDLE;
		this->m_STMT = SQL_NULL_HANDLE;
		this->m_RowCount = -1;
		this->m_ColCount = -1;

		memset(this->m_SQLColName, 0, sizeof(this->m_SQLColName));
		memset(this->m_SQLData, 0, sizeof(this->m_SQLData));

		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->m_SQLEnvironment);
		SQLSetEnvAttr(this->m_SQLEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	};

	~CQueryAsyncManager() {
		this->Disconnect();
	}

	bool Connect(char* odbc, char* user, char* pass);
	void Disconnect();
	void Diagnostic();
	bool ExecQuery(std::string query);
	void Close();
	SQLRETURN Fetch();
	int FindIndex(char* ColName);
	int GetResult(int index);
	int GetAsInteger(char* ColName);
	float GetAsFloat(char* ColName);
	__int64 GetAsInteger64(char* ColName);
	void GetAsString(char* ColName, char* OutBuffer, int OutBufferSize);
	void GetAsBinary(char* ColName, BYTE* OutBuffer, int OutBufferSize);
	void BindParameterAsString(int ParamNumber, void* InBuffer, int ColumnSize);
	void BindParameterAsBinary(int ParamNumber, void* InBuffer, int ColumnSize);
	void ConvertStringToBinary(char* InBuff, int InSize, BYTE* OutBuff, int OutSize);
	void ConvertBinaryToString(BYTE* InBuff, int InSize, char* OutBuff, int OutSize);

	std::vector<std::string> GetColumnNames()
	{
		std::vector<std::string> columns;
		for (int n = 0; n < this->m_ColCount; n++)
		{
			columns.push_back(std::string((char*)&this->m_SQLColName[n]));
		}
		return columns;
	}

	std::string GetString(int index)
	{
		char data[sizeof(this->m_SQLData)] = { 0, };
		strncpy_s(data, sizeof(data), this->m_SQLData[index], (sizeof(data) - 1));
		return std::string(data);
	}

private:
	SQLHANDLE m_SQLEnvironment;
	SQLHANDLE m_SQLConnection;
	SQLHANDLE m_STMT;
	char m_odbc[32];
	char m_user[32];
	char m_pass[32];
	SQLINTEGER m_RowCount;
	SQLSMALLINT m_ColCount;
	SQLCHAR m_SQLColName[MAX_COLUMNS][30];
	char m_SQLData[MAX_COLUMNS][8192];
	SQLINTEGER m_SQLDataLen[MAX_COLUMNS];
	SQLINTEGER m_SQLBindValue[MAX_COLUMNS];
};