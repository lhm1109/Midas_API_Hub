#include "stdafx.h"
#include "DB_SIHO.h"

CDB_SIHO::CDB_SIHO()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_siho.InitHashTable(HASHSIZESIHO);
}

CDB_SIHO::~CDB_SIHO()
{
}

CDB_SIHO::CDB_SIHO(CDB_SIHO& src)
{
	*this = src;
}

CDB_SIHO& CDB_SIHO::operator= (CDB_SIHO& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_SIHO_K Key;
	T_SIHO_D Data;
	m_siho.RemoveAll();
	pos=src.m_siho.GetStartPosition();
	while(pos != NULL)
	{
		src.m_siho.GetNextAssoc(pos,Key,Data);
		m_siho.SetAt(Key, Data);
	}

	return *this;
}

void CDB_SIHO::Add(T_SIHO_K Key,T_SIHO_D& rData)
{
	T_SIHO_D data;
	BOOL bExist=m_siho.Lookup(Key, data);
	m_siho.SetAt(Key,rData);

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

BOOL CDB_SIHO::Del(T_SIHO_K Key)
{
	T_SIHO_D OldData;
	m_siho.Lookup(Key, OldData);
	BOOL ret=m_siho.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SIHO_K key;
			T_SIHO_D data;
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
