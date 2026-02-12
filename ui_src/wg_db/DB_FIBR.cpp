#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_FIBR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FIBR::CDB_FIBR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_fibr.InitHashTable(HASHSIZEFIBR);
}

CDB_FIBR::~CDB_FIBR()
{
}

void CDB_FIBR::Add(T_FIBR_K Key,T_FIBR_D& rData)
{
	m_fibr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_FIBR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FIBR::Del(T_FIBR_K Key)
{
	T_FIBR_D Data;
	BOOL bExist=m_fibr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fibr.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_FIBR_K key;
				T_FIBR_D data;
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
	return FALSE;
}

//-------------------------------------------------------------------------
BOOL CDB_FIBR::Get(T_FIBR_K Key,T_FIBR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fibr.Lookup(Key,rData);
}

int CDB_FIBR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fibr.GetCount();
}

POSITION CDB_FIBR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fibr.GetStartPosition();
}

void CDB_FIBR::GetNext(POSITION& rNextPosition,T_FIBR_K& rKey,T_FIBR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fibr.GetNextAssoc(rNextPosition,rKey,rData);
}

int CDB_FIBR::GetCount(int nAssignType)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;

	int nCount = 0;
	auto pAssoc = m_fibr.PGetFirstAssoc();
	while(pAssoc)
	{
		if(pAssoc->value.nAssignType == nAssignType)
			nCount++;

		pAssoc = m_fibr.PGetNextAssoc(pAssoc);
	}
	return nCount;
}
