#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_RDGN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RDGN::CDB_RDGN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rdgn.InitHashTable(HASHSIZERDGN);
}

CDB_RDGN::~CDB_RDGN()
{

}

void CDB_RDGN::Add(T_RDGN_K Key,T_RDGN_D& rData,CDB_SECT* pSect)
{
	T_RDGN_D Data;
	BOOL bExist=m_rdgn.Lookup(Key, Data);
	m_rdgn.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_RDGN_CMD, Key);
}

BOOL CDB_RDGN::Del(T_RDGN_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_rdgn.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_RDGN_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RDGN::Get(T_RDGN_K Key,T_RDGN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rdgn.Lookup(Key,rData);
}

int CDB_RDGN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rdgn.GetCount();
}

POSITION CDB_RDGN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rdgn.GetStartPosition();
}

void CDB_RDGN::GetNext(POSITION& rNextPosition,T_RDGN_K& rKey,T_RDGN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rdgn.GetNextAssoc(rNextPosition,rKey,rData);
}
