#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_TDAS.h"

CDB_TDAS::CDB_TDAS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdas.InitHashTable(HASHSIZETDAS);
}

CDB_TDAS::~CDB_TDAS()
{
}

void CDB_TDAS::Add(T_TDAS_K Key,T_TDAS_D& rData)
{
	m_tdas.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_TDAS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_tdas.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDAS::Del(T_TDAS_K Key)
{
	T_TDAS_D Data;
	BOOL bExist=m_tdas.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tdas.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_TDAS_K key;
				T_TDAS_D data;
				if(m_tdas.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_tdas.GetStartPosition();
					while(pos != NULL)
					{
						m_tdas.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
		}
		return ret;
	}
	return FALSE;
}

