#include "stdafx.h"
#include "DB_GSBG.h"

CDB_GSBG::CDB_GSBG()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_gsbg.InitHashTable(HASHSIZEGSBG);
}

CDB_GSBG::~CDB_GSBG()
{
}

void CDB_GSBG::Add(T_GSBG_K Key,T_GSBG_D& rData)
{
	m_gsbg.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GSBG_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GSBG::Del(T_GSBG_K Key)
{
	BOOL ret=m_gsbg.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GSBG_K key;
			T_GSBG_D data;
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