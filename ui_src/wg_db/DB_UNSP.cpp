#include "stdafx.h"
#include "DB_UNSP.h"

CDB_UNSP::CDB_UNSP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_unsp.InitHashTable(HASHSIZEUNSP);
}

CDB_UNSP::~CDB_UNSP()
{
}

void CDB_UNSP::Add(T_UNSP_K Key,T_UNSP_D& rData)
{
	m_unsp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_UNSP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_UNSP::Del(T_UNSP_K Key)
{
	BOOL ret=m_unsp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_UNSP_K key;
			T_UNSP_D data;
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