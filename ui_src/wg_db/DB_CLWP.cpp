#include "stdafx.h"
#include "DB_CLWP.h"

CDB_CLWP::CDB_CLWP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_clwp.InitHashTable(HASHSIZECLWP);
}

CDB_CLWP::~CDB_CLWP()
{
}

void CDB_CLWP::Add(T_CLWP_K Key,T_CLWP_D& rData)
{
	m_clwp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_CLWP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_CLWP::Del(T_CLWP_K Key)
{
	BOOL ret=m_clwp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CLWP_K key;
			T_CLWP_D data;
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

