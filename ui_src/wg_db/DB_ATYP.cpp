#include "stdafx.h"
#include "DB_ATYP.h"

CDB_ATYP::CDB_ATYP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_atyp.InitHashTable(HASHSIZEATYP);
}

CDB_ATYP::~CDB_ATYP()
{
}

CDB_ATYP::CDB_ATYP(CDB_ATYP& src)
{
	*this = src;
}

CDB_ATYP& CDB_ATYP::operator= (CDB_ATYP& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_ATYP_K KeyEpmt;
	T_ATYP_D DataEpmt;
	m_atyp.RemoveAll();
	pos=src.m_atyp.GetStartPosition();
	while(pos != NULL)
	{
		src.m_atyp.GetNextAssoc(pos,KeyEpmt,DataEpmt);
		m_atyp.SetAt(KeyEpmt, DataEpmt);
	}

	return *this;
}

void CDB_ATYP::Add(T_ATYP_K Key,T_ATYP_D& rData)
{
	m_atyp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_ATYP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_ATYP::Del(T_ATYP_K Key)
{
	BOOL ret=m_atyp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ATYP_K key;
			T_ATYP_D data;
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
