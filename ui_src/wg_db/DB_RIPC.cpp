#include "stdafx.h"
#include "DB_RIPC.h"

CDB_RIPC::CDB_RIPC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ripc.InitHashTable(HASHSIZERIPC);
}

CDB_RIPC::~CDB_RIPC()
{
}

void CDB_RIPC::Add(T_RIPC_K Key,T_RIPC_D& rData)
{
	T_RIPC_D Data;
	BOOL bExist=m_ripc.Lookup(Key, Data);
	m_ripc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RIPC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RIPC::Del(T_RIPC_K Key)
{
	T_RIPC_D Data;
	BOOL bExist=m_ripc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ripc.RemoveKey(Key);
		ASSERT(ret);
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RIPC_K key;
			T_RIPC_D data;
			if(m_ripc.Lookup(m_nLastNum-1,data))
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
		return ret;
	}
	return bExist;
}