#include "stdafx.h"
#include "DB_ETFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ETFC::CDB_ETFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_etfc.InitHashTable(HASHSIZEETFC);
}

CDB_ETFC::~CDB_ETFC()
{
}

void CDB_ETFC::Add(T_ETFC_K Key,T_ETFC_D& rData)
{
	m_etfc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_ETFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_ETFC::Del(T_ETFC_K Key)
{
	BOOL ret=m_etfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ETFC_K key;
			T_ETFC_D data;
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
BOOL CDB_ETFC::Get(T_ETFC_K Key,T_ETFC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_etfc.Lookup(Key,rData);
}

int CDB_ETFC::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_etfc.GetCount();
}

POSITION CDB_ETFC::GetStart()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_etfc.GetStartPosition();
}

void CDB_ETFC::GetNext(POSITION& rNextPosition,T_ETFC_K& rKey,T_ETFC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//{
	//  ASSERT(0);
	//  return;
	//}
	m_etfc.GetNextAssoc(rNextPosition,rKey,rData);
}
