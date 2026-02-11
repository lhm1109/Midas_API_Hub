#include "stdafx.h"
#include "DB_SPFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPFC::CDB_SPFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_spfc.InitHashTable(HASHSIZESPFC);
}

CDB_SPFC::~CDB_SPFC()
{
}

// PostCtrl에서 복사본 만들어 사용할 때 사용
void CDB_SPFC::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_spfc.RemoveAll();
}

void CDB_SPFC::Add(T_SPFC_K Key,T_SPFC_D& rData)
{
	m_spfc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_SPFC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SPFC::Del(T_SPFC_K Key)
{
	BOOL ret=m_spfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SPFC_K key;
			T_SPFC_D data;
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
BOOL CDB_SPFC::Get(T_SPFC_K Key,T_SPFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_spfc.Lookup(Key,rData);
}

int CDB_SPFC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_spfc.GetCount();
}

POSITION CDB_SPFC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_spfc.GetStartPosition();
}

void CDB_SPFC::GetNext(POSITION& rNextPosition,T_SPFC_K& rKey,T_SPFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_spfc.GetNextAssoc(rNextPosition,rKey,rData);
}
