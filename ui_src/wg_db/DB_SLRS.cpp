#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SLRS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLRS::CDB_SLRS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_slrs.InitHashTable(HASHSIZESLRS);
}

CDB_SLRS::~CDB_SLRS()
{

}

void CDB_SLRS::Add(T_SLRS_K Key,T_SLRS_D& rData,CDB_ELEM* pElem)
{
	T_SLRS_D Data;
	BOOL bExist=m_slrs.Lookup(Key, Data);
	m_slrs.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SLRS_CMD, Key);
}

BOOL CDB_SLRS::Del(T_SLRS_K Key,CDB_ELEM* pElem)
{
	T_SLRS_D Data;
	BOOL bExist=m_slrs.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_slrs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_SLRS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SLRS::Get(T_SLRS_K Key,T_SLRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_slrs.Lookup(Key,rData);
}

int CDB_SLRS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_slrs.GetCount();
}

POSITION CDB_SLRS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_slrs.GetStartPosition();
}

void CDB_SLRS::GetNext(POSITION& rNextPosition,T_SLRS_K& rKey,T_SLRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_slrs.GetNextAssoc(rNextPosition,rKey,rData);
}
