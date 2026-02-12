#include "stdafx.h"
#include "DB_SIHG.h"

CDB_SIHG::CDB_SIHG()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_sihg.InitHashTable(HASHSIZESIHG);
}

CDB_SIHG::~CDB_SIHG()
{
}

CDB_SIHG::CDB_SIHG(CDB_SIHG& src)
{
	*this = src;
}

CDB_SIHG& CDB_SIHG::operator= (CDB_SIHG& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_SIHG_K Key;
	T_SIHG_D Data;
	m_sihg.RemoveAll();
	pos=src.m_sihg.GetStartPosition();
	while(pos != NULL)
	{
		src.m_sihg.GetNextAssoc(pos,Key,Data);
		m_sihg.SetAt(Key, Data);
	}

	return *this;
}

void CDB_SIHG::Add(T_SIHG_K Key,T_SIHG_D& rData)
{
	T_SIHG_D data;
	BOOL bExist=m_sihg.Lookup(Key, data);
	m_sihg.SetAt(Key,rData);

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

BOOL CDB_SIHG::Del(T_SIHG_K Key)
{
	T_SIHG_D OldData;
	m_sihg.Lookup(Key, OldData);
	BOOL ret=m_sihg.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SIHG_K key;
			T_SIHG_D data;
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
