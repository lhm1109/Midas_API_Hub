#include "stdafx.h"
#include "DB_LLCD.h"
#include "DB_STLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LLCD::CDB_LLCD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_LLCD.InitHashTable(HASHSIZELLCD);
}

CDB_LLCD::~CDB_LLCD()
{
	
}

void CDB_LLCD::Add(T_LLCD_K Key,T_LLCD_D& rData,CDB_STLD* pStld)
{
	T_LLCD_D Data;
	BOOL bExist=m_LLCD.Lookup(Key, Data);
	m_LLCD.SetAt(Key,rData);
	if(!bExist && pStld)pStld->AddListItem(Key, LT_LLCD_CMD, Key);
}

BOOL CDB_LLCD::Del(T_LLCD_K Key,CDB_STLD* pStld)
{
	BOOL ret=m_LLCD.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pStld)VERIFY(pStld->DelListItem(Key, LT_LLCD_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LLCD::Get(T_LLCD_K Key,T_LLCD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_LLCD.Lookup(Key,rData);
}

int CDB_LLCD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_LLCD.GetCount();
}

POSITION CDB_LLCD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_LLCD.GetStartPosition();
}

void CDB_LLCD::GetNext(POSITION& rNextPosition,T_LLCD_K& rKey,T_LLCD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_LLCD.GetNextAssoc(rNextPosition,rKey,rData);
}
