#include "stdafx.h"
#include "DB_MLFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MLFC::CDB_MLFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mlfc.InitHashTable(HASHSIZEMLFC);
}

CDB_MLFC::~CDB_MLFC()
{
}

void CDB_MLFC::Add(T_MLFC_K Key,T_MLFC_D& rData)
{
	m_mlfc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MLFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MLFC::Del(T_MLFC_K Key)
{
	BOOL ret=m_mlfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MLFC_K key;
			T_MLFC_D data;
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
BOOL CDB_MLFC::Get(T_MLFC_K Key,T_MLFC_D& rData)
{
	return m_mlfc.Lookup(Key,rData);
}

int CDB_MLFC::GetCount()
{
	return m_mlfc.GetCount();
}

POSITION CDB_MLFC::GetStart()
{
	return m_mlfc.GetStartPosition();
}

void CDB_MLFC::GetNext(POSITION& rNextPosition,T_MLFC_K& rKey,T_MLFC_D& rData)
{
	m_mlfc.GetNextAssoc(rNextPosition,rKey,rData);
}