#include "stdafx.h"
#include "DB_SSRI.h"
#include "DB_ELEM.h"

CDB_SSRI::CDB_SSRI()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_ssri.InitHashTable(HASHSIZESSRI);
}

CDB_SSRI::~CDB_SSRI()
{
}

void CDB_SSRI::Add(T_SSRI_K Key,T_SSRI_D& rData, CDB_ELEM* pElem)
{
	T_SSRI_D Data;
	BOOL bExist=m_ssri.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pElem->DelListItem(Key, LT_SSRI_CMD, Key));
	}
	m_ssri.SetAt(Key,rData);
	if (pElem)
	{
		pElem->AddListItem(Key, LT_SSRI_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_SSRI_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SSRI::Del(T_SSRI_K Key, CDB_ELEM* pElem)
{
	T_SSRI_D Data;
	BOOL bExist=m_ssri.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ssri.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)
			{
				VERIFY(pElem->DelListItem(Key, LT_SSRI_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SSRI_K key;
			T_SSRI_D data;
			if(m_ssri.Lookup(m_nLastNum-1,data))
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