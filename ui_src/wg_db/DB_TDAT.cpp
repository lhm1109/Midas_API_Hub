#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_TDAT.h"

CDB_TDAT::CDB_TDAT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdat.InitHashTable(HASHSIZETDAT);
}

CDB_TDAT::~CDB_TDAT()
{
}

void CDB_TDAT::Add(T_TDAT_K Key,T_TDAT_D& rData)
{
	m_tdat.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_TDAT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_tdat.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDAT::Del(T_TDAT_K Key)
{
	T_TDAT_D Data;
	BOOL bExist=m_tdat.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_tdat.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_TDAT_K key;
				T_TDAT_D data;
				if(m_tdat.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_tdat.GetStartPosition();
					while(pos != NULL)
					{
						m_tdat.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}
		}
		return ret;
	}
	return FALSE;
}

