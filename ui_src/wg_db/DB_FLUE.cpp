#include "stdafx.h"
#include "DB_FLUE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FLUE::CDB_FLUE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_flue.InitHashTable(HASHSIZEFLUE);
}

CDB_FLUE::~CDB_FLUE()
{
}

void CDB_FLUE::Initialize()
{
	ASSERT(m_pDoc == 0);
	m_nStartNum=1;
	m_nLastNum=0;
	m_flue.RemoveAll();
}

void CDB_FLUE::Add(T_FLUE_K Key,T_FLUE_D& rData)
{
	m_flue.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_FLUE_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FLUE::Del(T_FLUE_K Key)
{
	BOOL ret=m_flue.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_FLUE_K key;
			T_FLUE_D data;
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
BOOL CDB_FLUE::Get(T_FLUE_K Key,T_FLUE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_flue.Lookup(Key,rData);
}

int CDB_FLUE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_flue.GetCount();
}

POSITION CDB_FLUE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_flue.GetStartPosition();
}

void CDB_FLUE::GetNext(POSITION& rNextPosition,T_FLUE_K& rKey,T_FLUE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_flue.GetNextAssoc(rNextPosition,rKey,rData);
}

