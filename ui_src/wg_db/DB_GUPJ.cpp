#include "stdafx.h"
#include "DB_GUPJ.h"
#include "DB_PJCF.h"

CDB_GUPJ::CDB_GUPJ()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_gupj.InitHashTable(HASHSIZEGUPJ);
}

CDB_GUPJ::~CDB_GUPJ()
{
}

void CDB_GUPJ::Add(T_GUPJ_K Key,T_GUPJ_D& rData, CDB_PJCF* pPjcf)
{
	T_GUPJ_D Data;
	BOOL bExist=m_gupj.Lookup(Key, Data);
	m_gupj.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GUPJ_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GUPJ::Del(T_GUPJ_K Key, CDB_PJCF* pPjcf)
{
	T_GUPJ_D Data;
	BOOL bExist=m_gupj.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_gupj.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GUPJ_K key;
			T_GUPJ_D data;
			if(m_gupj.Lookup(m_nLastNum-1,data))
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