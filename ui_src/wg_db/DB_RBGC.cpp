#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_RBGC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBGC::CDB_RBGC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbgc.InitHashTable(HASHSIZERBGC);
}

CDB_RBGC::~CDB_RBGC()
{

}

void CDB_RBGC::Add(T_RBGC_K Key,T_RBGC_D& rData,CDB_SECT* pSect)
{
	T_RBGC_D Data;
	BOOL bExist=m_rbgc.Lookup(Key, Data);
	m_rbgc.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_RBGC_CMD, Key);
}

BOOL CDB_RBGC::Del(T_RBGC_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rbgc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_RBGC_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RBGC::Get(T_RBGC_K Key,T_RBGC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rbgc.Lookup(Key,rData);
}

int CDB_RBGC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbgc.GetCount();
}

POSITION CDB_RBGC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rbgc.GetStartPosition();
}

void CDB_RBGC::GetNext(POSITION& rNextPosition,T_RBGC_K& rKey,T_RBGC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rbgc.GetNextAssoc(rNextPosition,rKey,rData);
}
