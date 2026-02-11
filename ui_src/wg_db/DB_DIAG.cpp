#include "stdafx.h"
#include "DB_DIAG.h"

CDB_DIAG::CDB_DIAG()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_diag.InitHashTable(HASHSIZEDIAG);
}

CDB_DIAG::~CDB_DIAG()
{
}

void CDB_DIAG::Add(T_DIAG_K Key,T_DIAG_D& rData)
{
	T_DIAG_D Data;
	BOOL bExist = m_diag.Lookup(Key, Data);
	m_diag.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_DIAG_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DIAG::Del(T_DIAG_K Key)
{
	BOOL ret=m_diag.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_DIAG_K key;
			T_DIAG_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

