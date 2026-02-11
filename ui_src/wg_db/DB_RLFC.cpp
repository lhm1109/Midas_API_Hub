#include "stdafx.h"
#include "DB_RLFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RLFC::CDB_RLFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_RLFC.InitHashTable(HASHSIZERLFC);
}

CDB_RLFC::~CDB_RLFC()
{
}

void CDB_RLFC::Add(T_RLFC_K Key,T_RLFC_D& rData)
{
	m_RLFC.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_RLFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_RLFC::Del(T_RLFC_K Key)
{
	BOOL ret=m_RLFC.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_RLFC_K key;
			T_RLFC_D data;
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

//-------------------------------------------------------------------------
BOOL CDB_RLFC::Get(T_RLFC_K Key,T_RLFC_D& rData)
{
	return m_RLFC.Lookup(Key,rData);
}

int CDB_RLFC::GetCount()
{
	return m_RLFC.GetCount();
}

POSITION CDB_RLFC::GetStart()
{
	return m_RLFC.GetStartPosition();
}

void CDB_RLFC::GetNext(POSITION& rNextPosition,T_RLFC_K& rKey,T_RLFC_D& rData)
{
	m_RLFC.GetNextAssoc(rNextPosition,rKey,rData);
}