#include "stdafx.h"
#include "DB_COLC.h"
#include "DB_ELEM.h"

CDB_COLC::CDB_COLC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_colc.InitHashTable(HASHSIZECOLC);
}

CDB_COLC::~CDB_COLC()
{
}

void CDB_COLC::Add(T_COLC_K Key,T_COLC_D& rData, CDB_ELEM* pElem)
{
	T_COLC_D Data;
	BOOL bExist=m_colc.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_COLC_CMD, Key));
	}
	m_colc.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_COLC_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_COLC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_COLC::Del(T_COLC_K Key, CDB_ELEM* pElem)
{
	T_COLC_D Data;
	BOOL bExist=m_colc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_colc.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_COLC_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_COLC_K key;
			T_COLC_D data;
			if(m_colc.Lookup(m_nLastNum-1,data))
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