#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RCCO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCCO::CDB_RCCO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcco.InitHashTable(HASHSIZERCCO);
}

CDB_RCCO::~CDB_RCCO()
{

}

void CDB_RCCO::Add(T_RCCO_K Key,T_RCCO_D& rData,CDB_ELEM* pElem)
{
	T_RCCO_D Data;
	BOOL bExist=m_rcco.Lookup(Key, Data);
	m_rcco.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RCCO_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RCCO_CMD, Key);
	}
}

BOOL CDB_RCCO::Del(T_RCCO_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rcco.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RCCO_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RCCO_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RCCO::Get(T_RCCO_K Key,T_RCCO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rcco->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rcco.Lookup(Key,rData);
}

int CDB_RCCO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcco->GetCount();
	}
	return m_rcco.GetCount();
}

POSITION CDB_RCCO::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcco->GetStartPosition();
	}
	return m_rcco.GetStartPosition();
}

void CDB_RCCO::GetNext(POSITION& rNextPosition,T_RCCO_K& rKey,T_RCCO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rcco->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rcco.Lookup(rKey, rData);
		return;
	}
	m_rcco.GetNextAssoc(rNextPosition,rKey,rData);
}
