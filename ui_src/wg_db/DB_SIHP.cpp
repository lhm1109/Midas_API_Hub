#include "stdafx.h"
#include "DB_SIHP.h"

CDB_SIHP::CDB_SIHP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sihp.InitHashTable(HASHSIZESIHP);
}

CDB_SIHP::~CDB_SIHP()
{
}

CDB_SIHP::CDB_SIHP(CDB_SIHP& src)
{
	*this = src;
}

CDB_SIHP& CDB_SIHP::operator= (CDB_SIHP& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_SIHP_K Key;
	T_SIHP_D Data;
	m_sihp.RemoveAll();
	pos=src.m_sihp.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sihp.GetNextAssoc(pos,Key,Data);
		m_sihp.SetAt(Key, Data);
	}

	return *this;
}

void CDB_SIHP::Add(T_SIHP_K Key,T_SIHP_D& rData)
{
	T_SIHP_D data;
	BOOL bExist=m_sihp.Lookup(Key, data);
	m_sihp.SetAt(Key,rData);

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

BOOL CDB_SIHP::Del(T_SIHP_K Key)
{
	T_SIHP_D OldData;
	m_sihp.Lookup(Key, OldData);
	BOOL ret=m_sihp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SIHP_K key;
			T_SIHP_D data;
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
