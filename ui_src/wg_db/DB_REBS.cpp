#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_REBS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REBS::CDB_REBS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rebs.InitHashTable(HASHSIZEREBS);
}

CDB_REBS::~CDB_REBS()
{

}

void CDB_REBS::Add(T_REBS_K Key,T_REBS_D& rData,CDB_SECT* pSect)
{
	T_REBS_D Data;
	BOOL bExist=m_rebs.Lookup(Key, Data);
	m_rebs.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_REBS_CMD, Key);
}

BOOL CDB_REBS::Del(T_REBS_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rebs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_REBS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REBS::Get(T_REBS_K Key,T_REBS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rebs.Lookup(Key,rData);
}

int CDB_REBS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebs.GetCount();
}

POSITION CDB_REBS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rebs.GetStartPosition();
}

void CDB_REBS::GetNext(POSITION& rNextPosition,T_REBS_K& rKey,T_REBS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rebs.GetNextAssoc(rNextPosition,rKey,rData);
}
