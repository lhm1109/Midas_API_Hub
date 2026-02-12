#include "stdafx.h"
#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_SLSD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLSD::CDB_SLSD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_slsd.InitHashTable(HASHSIZESLSD);
}

CDB_SLSD::~CDB_SLSD()
{

}

void CDB_SLSD::Add(T_SLSD_K Key,T_SLSD_D& rData, CDB_SECT* pSect)
{
	T_SLSD_D Data;
	BOOL bExist=m_slsd.Lookup(Key, Data);
	m_slsd.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_SLSD_CMD, Key);
}

BOOL CDB_SLSD::Del(T_SLSD_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_slsd.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pSect->DelListItem(Key, LT_SLSD_CMD, Key));
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SLSD::Get(T_SLSD_K Key,T_SLSD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_slsd.Lookup(Key,rData);
}

int CDB_SLSD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_slsd.GetCount();
}

POSITION CDB_SLSD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_slsd.GetStartPosition();
}

void CDB_SLSD::GetNext(POSITION& rNextPosition,T_SLSD_K& rKey,T_SLSD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_slsd.GetNextAssoc(rNextPosition,rKey,rData);
}
