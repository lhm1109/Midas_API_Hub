#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_STFN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STFN::CDB_STFN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stfn.InitHashTable(HASHSIZESTFN);
}

CDB_STFN::~CDB_STFN()
{

}

void CDB_STFN::Add(T_STFN_K Key,T_STFN_D& rData,CDB_SECT* pSect)
{
	T_STFN_D Data;
	BOOL bExist=m_stfn.Lookup(Key, Data);
	m_stfn.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_STFN_CMD, Key);
}

BOOL CDB_STFN::Del(T_STFN_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_stfn.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_STFN_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STFN::Get(T_STFN_K Key,T_STFN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_stfn.Lookup(Key,rData);
}

int CDB_STFN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stfn.GetCount();
}

POSITION CDB_STFN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stfn.GetStartPosition();
}

void CDB_STFN::GetNext(POSITION& rNextPosition,T_STFN_K& rKey,T_STFN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_stfn.GetNextAssoc(rNextPosition,rKey,rData);
}
