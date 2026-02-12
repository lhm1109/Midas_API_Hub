#include "stdafx.h"
#include "DB_TDCS.h"

CDB_TDCS::CDB_TDCS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdcs.InitHashTable(HASHSIZETDCS);
}

CDB_TDCS::~CDB_TDCS()
{
}

CDB_TDCS::CDB_TDCS(CDB_TDCS& src)
{
	*this = src;
}

CDB_TDCS& CDB_TDCS::operator= (CDB_TDCS& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_TDCS_K Key;
	T_TDCS_D Data;
	m_tdcs.RemoveAll();
	pos=src.m_tdcs.GetStartPosition();
	while(pos != NULL)
	{
		src.m_tdcs.GetNextAssoc(pos,Key,Data);
		m_tdcs.SetAt(Key, Data);
	}

	return *this;
}

void CDB_TDCS::Add(T_TDCS_K Key,T_TDCS_D& rData)
{
	T_TDCS_D data;
	BOOL bExist=m_tdcs.Lookup(Key, data);
	m_tdcs.SetAt(Key,rData);
	
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDCS::Del(T_TDCS_K Key)
{
	T_TDCS_D OldData;
	m_tdcs.Lookup(Key, OldData);
	BOOL ret=m_tdcs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDCS_K key;
			T_TDCS_D data;
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
