#include "stdafx.h"
#include "DB_CSCS.h"
#include "DB_SECT.h"

CDB_CSCS::CDB_CSCS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_cscs.InitHashTable(HASHSIZECSCS);
}

CDB_CSCS::~CDB_CSCS()
{
}

CDB_CSCS::CDB_CSCS(CDB_CSCS& src)
{
	*this = src;
}

CDB_CSCS& CDB_CSCS::operator= (CDB_CSCS& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_CSCS_K KeySect;
	T_CSCS_D DataSect;
	m_cscs.RemoveAll();
	pos=src.m_cscs.GetStartPosition();
	while(pos != NULL)
	{
		src.m_cscs.GetNextAssoc(pos,KeySect,DataSect);
		m_cscs.SetAt(KeySect, DataSect);
	}

	return *this;
}

void CDB_CSCS::Add(T_CSCS_K Key,T_CSCS_D& rData,CDB_SECT* pSect)
{
	T_CSCS_D data;
	BOOL bExist=m_cscs.Lookup(Key, data);
	m_cscs.SetAt(Key,rData);
	if(pSect != NULL)
	{
		if (bExist && rData.SectKey != data.SectKey)
			pSect->DelListItem(data.SectKey, LT_CSCS_CMD, Key);
		if (!bExist || rData.SectKey != data.SectKey)
			pSect->AddListItem(rData.SectKey, LT_CSCS_CMD, Key);
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

BOOL CDB_CSCS::Del(T_CSCS_K Key,CDB_SECT* pSect)
{
	T_CSCS_D OldData;
	m_cscs.Lookup(Key, OldData);
	BOOL ret=m_cscs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(pSect != NULL)VERIFY(pSect->DelListItem(OldData.SectKey, LT_CSCS_CMD, Key));

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CSCS_K key;
			T_CSCS_D data;
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
