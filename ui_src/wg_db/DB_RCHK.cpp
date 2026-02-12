#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_RCHK.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCHK::CDB_RCHK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rchk.InitHashTable(HASHSIZERCHK);
}

CDB_RCHK::~CDB_RCHK()
{

}

void CDB_RCHK::Add(T_RCHK_K Key,T_RCHK_D& rData,CDB_SECT* pSect)
{
	T_RCHK_D Data;
	BOOL bExist=m_rchk.Lookup(Key, Data);
	m_rchk.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_RCHK_CMD, Key);
}

BOOL CDB_RCHK::Del(T_RCHK_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rchk.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_RCHK_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RCHK::Get(T_RCHK_K Key,T_RCHK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rchk.Lookup(Key,rData);
}

int CDB_RCHK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rchk.GetCount();
}

POSITION CDB_RCHK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rchk.GetStartPosition();
}

void CDB_RCHK::GetNext(POSITION& rNextPosition,T_RCHK_K& rKey,T_RCHK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rchk.GetNextAssoc(rNextPosition,rKey,rData);
}
