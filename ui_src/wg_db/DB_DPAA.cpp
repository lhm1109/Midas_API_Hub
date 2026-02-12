#include "stdafx.h"
#include "DB_DPAA.h"
#include "DB_ELEM.h"

CDB_DPAA::CDB_DPAA()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_dpaa.InitHashTable(HASHSIZEDPAA);
}

CDB_DPAA::~CDB_DPAA()
{
}

void CDB_DPAA::Add(T_DPAA_K Key,T_DPAA_D& rData, CDB_ELEM* pElem)
{
	T_DPAA_D Data;
	BOOL bExist=m_dpaa.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_DPAA_CMD, Key));
	}
	m_dpaa.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_DPAA_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_DPAA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DPAA::Del(T_DPAA_K Key, CDB_ELEM* pElem)
{
	T_DPAA_D Data;
	BOOL bExist=m_dpaa.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dpaa.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_DPAA_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_DPAA_K key;
			T_DPAA_D data;
			if(m_dpaa.Lookup(m_nLastNum-1,data))
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