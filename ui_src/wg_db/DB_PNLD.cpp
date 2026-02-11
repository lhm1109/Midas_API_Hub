#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PNLD.h"

CDB_PNLD::CDB_PNLD()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pnld.InitHashTable(HASHSIZEPNLD);
}

CDB_PNLD::~CDB_PNLD()
{
}

void CDB_PNLD::Add(T_PNLD_K Key,T_PNLD_D& rData)
{
	m_pnld.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PNLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_pnld.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PNLD::Del(T_PNLD_K Key)
{
	T_PNLD_D Data;
	BOOL bExist=m_pnld.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pnld.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_PNLD_K key;
				T_PNLD_D data;
				if(m_pnld.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_pnld.GetStartPosition();
					while(pos != NULL)
					{
						m_pnld.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
		}
		return ret;
	}
	return FALSE;
}

