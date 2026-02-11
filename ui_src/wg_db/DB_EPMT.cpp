#include "stdafx.h"
#include "DB_EPMT.h"

CDB_EPMT::CDB_EPMT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_epmt.InitHashTable(HASHSIZEEPMT);
}

CDB_EPMT::~CDB_EPMT()
{
}

CDB_EPMT::CDB_EPMT(CDB_EPMT& src)
{
	*this = src;
}

CDB_EPMT& CDB_EPMT::operator= (CDB_EPMT& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_EPMT_K KeyEpmt;
	T_EPMT_D DataEpmt;
	m_epmt.RemoveAll();
	pos=src.m_epmt.GetStartPosition();
	while(pos != NULL)
	{
		src.m_epmt.GetNextAssoc(pos,KeyEpmt,DataEpmt);
		m_epmt.SetAt(KeyEpmt, DataEpmt);
	}

	return *this;
}

void CDB_EPMT::Add(T_EPMT_K Key,T_EPMT_D& rData)
{
	m_epmt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_EPMT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_EPMT::Del(T_EPMT_K Key)
{
	BOOL ret=m_epmt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_EPMT_K key;
			T_EPMT_D data;
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

