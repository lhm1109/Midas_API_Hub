#include "stdafx.h"
#include "DB_NPLN.h"

CDB_NPLN::CDB_NPLN()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_npln.InitHashTable(HASHSIZENPLN);
}

CDB_NPLN::~CDB_NPLN()
{
}

void CDB_NPLN::Add(T_NPLN_K Key,T_NPLN_D& rData)
{
	m_npln.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_NPLN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_NPLN::Del(T_NPLN_K Key)
{
	BOOL ret=m_npln.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_NPLN_K key;
			T_NPLN_D data;
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

