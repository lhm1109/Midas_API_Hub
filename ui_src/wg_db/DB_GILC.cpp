#include "stdafx.h"
#include "DB_GILC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GILC::CDB_GILC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_gilc.InitHashTable(HASHSIZEGILC);
}

CDB_GILC::~CDB_GILC()
{
}

// PostCtrl에서 복사본 만들어 사용할 때 사용
void CDB_GILC::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_gilc.RemoveAll();
}

void CDB_GILC::Add(T_GILC_K Key,T_GILC_D& rData)
{
	m_gilc.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_GILC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_GILC::Del(T_GILC_K Key)
{
	BOOL ret=m_gilc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_GILC_K key;
			T_GILC_D data;
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
BOOL CDB_GILC::Get(T_GILC_K Key,T_GILC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_gilc.Lookup(Key,rData);
}

int CDB_GILC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_gilc.GetCount();
}

POSITION CDB_GILC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_gilc.GetStartPosition();
}

void CDB_GILC::GetNext(POSITION& rNextPosition,T_GILC_K& rKey,T_GILC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_gilc.GetNextAssoc(rNextPosition,rKey,rData);
}
