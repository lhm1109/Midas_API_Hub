#include "stdafx.h"
#include "DB_RCCM.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCCM::CDB_RCCM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_RCCM.InitHashTable(HASHSIZERCCM);
}

CDB_RCCM::~CDB_RCCM()
{

}

void CDB_RCCM::Add(T_RCCM_K Key,T_RCCM_D& rData,CDB_ELEM* pElem)
{
	T_RCCM_D Data;
	BOOL bExist=m_RCCM.Lookup(Key, Data);
	m_RCCM.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_RCCM_CMD, Key);
}

BOOL CDB_RCCM::Del(T_RCCM_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_RCCM.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_RCCM_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RCCM::Get(T_RCCM_K Key,T_RCCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_RCCM.Lookup(Key,rData);
}

int CDB_RCCM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_RCCM.GetCount();
}

POSITION CDB_RCCM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_RCCM.GetStartPosition();
}

void CDB_RCCM::GetNext(POSITION& rNextPosition,T_RCCM_K& rKey,T_RCCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_RCCM.GetNextAssoc(rNextPosition,rKey,rData);
}
