#include "stdafx.h"
#include "DB_WNAT.h"
#include "DB_STLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WNAT::CDB_WNAT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_wnat.InitHashTable(HASHSIZEWNAT);
}

CDB_WNAT::~CDB_WNAT()
{
	
}

void CDB_WNAT::Add(T_WNAT_K Key,T_WNAT_D& rData,CDB_STLD* pStld)
{
	T_WNAT_D Data;
	BOOL bExist=m_wnat.Lookup(Key, Data);
	m_wnat.SetAt(Key,rData);
	if(!bExist && pStld)pStld->AddListItem(Key, LT_WNAT_CMD, Key);
}

BOOL CDB_WNAT::Del(T_WNAT_K Key,CDB_STLD* pStld)
{
	BOOL ret=m_wnat.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pStld)VERIFY(pStld->DelListItem(Key, LT_WNAT_CMD, Key));
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_WNAT::Get(T_WNAT_K Key,T_WNAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_wnat.Lookup(Key,rData);
}
int CDB_WNAT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wnat.GetCount();
}

POSITION CDB_WNAT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wnat.GetStartPosition();
}

void CDB_WNAT::GetNext(POSITION& rNextPosition,T_WNAT_K& rKey,T_WNAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_wnat.GetNextAssoc(rNextPosition,rKey,rData);
}
