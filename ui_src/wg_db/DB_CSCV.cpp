#include "stdafx.h"
#include "DB_CSCV.h"
#include "DB_SECV.h"

CDB_CSCV::CDB_CSCV()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_cscv.InitHashTable(HASHSIZECSCV);
}

CDB_CSCV::~CDB_CSCV()
{
}

CDB_CSCV::CDB_CSCV(CDB_CSCV& src)
{
	*this = src;
}

CDB_CSCV& CDB_CSCV::operator= (CDB_CSCV& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_CSCV_K KeySect;
	T_CSCV_D DataSect;
	m_cscv.RemoveAll();
	pos=src.m_cscv.GetStartPosition();
	while(pos != NULL)
	{
		src.m_cscv.GetNextAssoc(pos,KeySect,DataSect);
		m_cscv.SetAt(KeySect, DataSect);
	}

	return *this;
}

void CDB_CSCV::Add(T_CSCV_K Key,T_CSCV_D& rData,CDB_SECV* pSect)
{
	T_CSCV_D data;
	BOOL bExist=m_cscv.Lookup(Key, data);
	m_cscv.SetAt(Key,rData);
	if(pSect != NULL)
	{
		if (bExist && rData.SectKey != data.SectKey)
			pSect->DelListItem(data.SectKey, LT_CSCV_CMD, Key);
		if (!bExist || rData.SectKey != data.SectKey)
			pSect->AddListItem(rData.SectKey, LT_CSCV_CMD, Key);
	}
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

BOOL CDB_CSCV::Del(T_CSCV_K Key,CDB_SECV* pSect)
{
	T_CSCV_D OldData;
	m_cscv.Lookup(Key, OldData);
	BOOL ret=m_cscv.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(pSect != NULL)VERIFY(pSect->DelListItem(OldData.SectKey, LT_CSCV_CMD, Key));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CSCV_K key;
			T_CSCV_D data;
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
