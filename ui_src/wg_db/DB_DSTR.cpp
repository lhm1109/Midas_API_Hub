#include "stdafx.h"
#include "DB_DSTR.h"

CDB_DSTR::CDB_DSTR()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_dstr.InitHashTable(HASHSIZEDSTR);
}

CDB_DSTR::~CDB_DSTR()
{
}

void CDB_DSTR::Add(T_DSTR_K Key, T_DSTR_D& rData)
{
	T_DSTR_D Data;
	BOOL bExist=m_dstr.Lookup(Key, Data);
	m_dstr.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_DSTR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DSTR::Del(T_DSTR_K Key)
{
	T_DSTR_D Data;
	BOOL bExist=m_dstr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dstr.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_DSTR_K key;
			T_DSTR_D data;
			if(m_dstr.Lookup(m_nLastNum-1,data))
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
