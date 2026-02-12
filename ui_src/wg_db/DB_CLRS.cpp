#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_CLRS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CLRS::CDB_CLRS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_clrs.InitHashTable(HASHSIZECLRS);
}

CDB_CLRS::~CDB_CLRS()
{

}

void CDB_CLRS::Add(T_CLRS_K Key,T_CLRS_D& rData,CDB_SECT* pSect)
{
	T_CLRS_D Data;
	BOOL bExist=m_clrs.Lookup(Key, Data);
	m_clrs.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_CLRS_CMD, Key);
}

BOOL CDB_CLRS::Del(T_CLRS_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_clrs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_CLRS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CLRS::Get(T_CLRS_K Key,T_CLRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_clrs.Lookup(Key,rData);
}

int CDB_CLRS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_clrs.GetCount();
}

POSITION CDB_CLRS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_clrs.GetStartPosition();
}

void CDB_CLRS::GetNext(POSITION& rNextPosition,T_CLRS_K& rKey,T_CLRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_clrs.GetNextAssoc(rNextPosition,rKey,rData);
}
