#include "stdafx.h"
#include "DB_NUCS.h"

CDB_NUCS::CDB_NUCS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_nucs.InitHashTable(HASHSIZENUCS);
}

CDB_NUCS::~CDB_NUCS()
{
}

void CDB_NUCS::Add(T_NUCS_K Key,T_NUCS_D& rData)
{
	m_nucs.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_NUCS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_NUCS::Del(T_NUCS_K Key)
{
	BOOL ret=m_nucs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_NUCS_K key;
			T_NUCS_D data;
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

