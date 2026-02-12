#include "stdafx.h"
#include "DB_MPCT.h"

CDB_MPCT::CDB_MPCT()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_mpct.InitHashTable(HASHSIZEMPCT);
}

CDB_MPCT::~CDB_MPCT()
{
}

CDB_MPCT::CDB_MPCT(CDB_MPCT& src)
{
	*this = src;
}

CDB_MPCT& CDB_MPCT::operator= (CDB_MPCT& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_MPCT_K KeyEpmt;
	T_MPCT_D DataEpmt;
	m_mpct.RemoveAll();
	pos=src.m_mpct.GetStartPosition();
	while(pos != NULL)
	{
		src.m_mpct.GetNextAssoc(pos,KeyEpmt,DataEpmt);
		m_mpct.SetAt(KeyEpmt, DataEpmt);
	}

	return *this;
}

void CDB_MPCT::Add(T_MPCT_K Key,T_MPCT_D& rData)
{
	m_mpct.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MPCT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MPCT::Del(T_MPCT_K Key)
{
	BOOL ret=m_mpct.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MPCT_K key;
			T_MPCT_D data;
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

