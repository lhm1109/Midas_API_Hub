#include "stdafx.h"
#include "DB_SDST.h"
#include "DB_ELEM.h"

CDB_SDST::CDB_SDST()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sdst.InitHashTable(HASHSIZESDST);
}

CDB_SDST::~CDB_SDST()
{
}

void CDB_SDST::Add(T_SDST_K Key,T_SDST_D& rData)
{
	m_sdst.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SDST_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SDST::Del(T_SDST_K Key)
{
	T_SDST_D Data;
	BOOL bExist=m_sdst.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sdst.RemoveKey(Key);
		ASSERT(ret);
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SDST_K key;
			T_SDST_D data;
			if(m_sdst.Lookup(m_nLastNum-1,data))
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