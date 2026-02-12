#include "stdafx.h"
#include "DB_GRND.h"

CDB_GRND::CDB_GRND()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_grnd.InitHashTable(HASHSIZEGRND);
}

CDB_GRND::~CDB_GRND()
{
}

void CDB_GRND::Add(T_GRND_K Key,T_GRND_D& rData)
{
	m_grnd.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GRND_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GRND::Del(T_GRND_K Key)
{
	BOOL ret=m_grnd.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GRND_K key;
			T_GRND_D data;
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