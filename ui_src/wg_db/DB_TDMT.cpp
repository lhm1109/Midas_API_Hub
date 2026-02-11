#include "stdafx.h"
#include "DB_TDMT.h"

CDB_TDMT::CDB_TDMT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdmt.InitHashTable(HASHSIZETDMT);
}

CDB_TDMT::~CDB_TDMT()
{
}

CDB_TDMT::CDB_TDMT(CDB_TDMT& src)
{
	*this = src;
}

CDB_TDMT& CDB_TDMT::operator= (CDB_TDMT& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_TDMT_K KeyTdmt;
	T_TDMT_D DataTdmt;
	m_tdmt.RemoveAll();
	pos=src.m_tdmt.GetStartPosition();
	while(pos != NULL)
	{
		src.m_tdmt.GetNextAssoc(pos,KeyTdmt,DataTdmt);
		m_tdmt.SetAt(KeyTdmt, DataTdmt);
	}

	return *this;
}

void CDB_TDMT::Add(T_TDMT_K Key,T_TDMT_D& rData)
{
	m_tdmt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_TDMT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDMT::Del(T_TDMT_K Key)
{
	BOOL ret=m_tdmt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDMT_K key;
			T_TDMT_D data;
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

