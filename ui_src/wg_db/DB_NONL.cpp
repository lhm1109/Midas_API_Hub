#include "stdafx.h"
#include "DB_NONL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NONL::CDB_NONL()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_nonl.InitHashTable(HASHSIZENONL);
}

CDB_NONL::~CDB_NONL()
{
}

CDB_NONL::CDB_NONL(CDB_NONL& src)
{
	*this = src;
}

CDB_NONL& CDB_NONL::operator= (CDB_NONL& src)
{
	POSITION pos;

	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_NONL_K KeyEpmt;
	T_NONL_D DataEpmt;
	m_nonl.RemoveAll();
	pos=src.m_nonl.GetStartPosition();
	while(pos != NULL)
	{
		src.m_nonl.GetNextAssoc(pos,KeyEpmt,DataEpmt);
		m_nonl.SetAt(KeyEpmt, DataEpmt);
	}

	return *this;
}

void CDB_NONL::Add(T_NONL_K Key,T_NONL_D& rData)
{
	m_nonl.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_NONL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_NONL::Del(T_NONL_K Key)
{
	BOOL ret=m_nonl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_NONL_K key;
			T_NONL_D data;
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
