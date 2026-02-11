#include "stdafx.h"
#include "DB_CCFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CCFC::CDB_CCFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ccfc.InitHashTable(HASHSIZECCFC);
}

CDB_CCFC::~CDB_CCFC()
{
}

void CDB_CCFC::Add(T_CCFC_K Key,T_CCFC_D& rData)
{
	m_ccfc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_CCFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_CCFC::Del(T_CCFC_K Key)
{
	BOOL ret=m_ccfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_CCFC_K key;
			T_CCFC_D data;
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
BOOL CDB_CCFC::Get(T_CCFC_K Key,T_CCFC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_ccfc.Lookup(Key,rData);
}

int CDB_CCFC::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_ccfc.GetCount();
}

POSITION CDB_CCFC::GetStart()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_ccfc.GetStartPosition();
}

void CDB_CCFC::GetNext(POSITION& rNextPosition,T_CCFC_K& rKey,T_CCFC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//{
	//  ASSERT(0);
	//  return;
	//}
	m_ccfc.GetNextAssoc(rNextPosition,rKey,rData);
}
