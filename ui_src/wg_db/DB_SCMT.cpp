#include "stdafx.h"
#include "DB_SCMT.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SCMT::CDB_SCMT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SCMT.InitHashTable(HASHSIZESCMT);
}

CDB_SCMT::~CDB_SCMT()
{

}

void CDB_SCMT::Add(T_SCMT_K Key,T_SCMT_D& rData,CDB_ELEM* pElem)
{
	T_SCMT_D Data;
	BOOL bExist=m_SCMT.Lookup(Key, Data);
	m_SCMT.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_SCMT_CMD, Key);
}

BOOL CDB_SCMT::Del(T_SCMT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_SCMT.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_SCMT_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SCMT::Get(T_SCMT_K Key,T_SCMT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_SCMT.Lookup(Key,rData);
}

int CDB_SCMT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SCMT.GetCount();
}

POSITION CDB_SCMT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SCMT.GetStartPosition();
}

void CDB_SCMT::GetNext(POSITION& rNextPosition,T_SCMT_K& rKey,T_SCMT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_SCMT.GetNextAssoc(rNextPosition,rKey,rData);
}
