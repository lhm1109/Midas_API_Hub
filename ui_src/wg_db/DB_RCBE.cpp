#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RCBE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCBE::CDB_RCBE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcbe.InitHashTable(HASHSIZERCBE);
}

CDB_RCBE::~CDB_RCBE()
{

}

void CDB_RCBE::Add(T_RCBE_K Key,T_RCBE_D& rData,CDB_ELEM* pElem)
{
	T_RCBE_D Data;
	BOOL bExist=m_rcbe.Lookup(Key, Data);
	m_rcbe.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RCBE_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RCBE_CMD, Key);
	}
}

BOOL CDB_RCBE::Del(T_RCBE_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rcbe.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RCBE_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RCBE_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RCBE::Get(T_RCBE_K Key,T_RCBE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rcbe->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rcbe.Lookup(Key,rData);
}

int CDB_RCBE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcbe->GetCount();
	}
	return m_rcbe.GetCount();
}

POSITION CDB_RCBE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcbe->GetStartPosition();
	}
	return m_rcbe.GetStartPosition();
}

void CDB_RCBE::GetNext(POSITION& rNextPosition,T_RCBE_K& rKey,T_RCBE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rcbe->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rcbe.Lookup(rKey, rData);
		return;
	}
	m_rcbe.GetNextAssoc(rNextPosition,rKey,rData);
}
