#include "stdafx.h"
#include "DB_HSFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HSFC::CDB_HSFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_hsfc.InitHashTable(HASHSIZEHSFC);
}

CDB_HSFC::~CDB_HSFC()
{
}

void CDB_HSFC::Add(T_HSFC_K Key,T_HSFC_D& rData)
{
	m_hsfc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_HSFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_HSFC::Del(T_HSFC_K Key)
{
	BOOL ret=m_hsfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_HSFC_K key;
			T_HSFC_D data;
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
BOOL CDB_HSFC::Get(T_HSFC_K Key,T_HSFC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_hsfc.Lookup(Key,rData);
}

int CDB_HSFC::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_hsfc.GetCount();
}

POSITION CDB_HSFC::GetStart()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_hsfc.GetStartPosition();
}

void CDB_HSFC::GetNext(POSITION& rNextPosition,T_HSFC_K& rKey,T_HSFC_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//{
	//  ASSERT(0);
	//  return;
	//}
	m_hsfc.GetNextAssoc(rNextPosition,rKey,rData);
}
