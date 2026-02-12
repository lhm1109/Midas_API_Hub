#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SUFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SUFC::CDB_SUFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sufc.InitHashTable(HASHSIZESUFC);
}

CDB_SUFC::~CDB_SUFC()
{

}

void CDB_SUFC::Add(T_SUFC_K Key,T_SUFC_D& rData,CDB_ELEM* pElem)
{
	T_SUFC_D Data;
	BOOL bExist=m_sufc.Lookup(Key, Data);
	m_sufc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SUFC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SUFC_CMD, Key);
	}
}

BOOL CDB_SUFC::Del(T_SUFC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sufc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SUFC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SUFC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SUFC::Get(T_SUFC_K Key,T_SUFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sufc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sufc.Lookup(Key,rData);
}

int CDB_SUFC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sufc->GetCount();
	}
	return m_sufc.GetCount();
}

POSITION CDB_SUFC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sufc->GetStartPosition();
	}
	return m_sufc.GetStartPosition();
}

void CDB_SUFC::GetNext(POSITION& rNextPosition,T_SUFC_K& rKey,T_SUFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sufc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sufc.Lookup(rKey, rData);
		return;
	}
	m_sufc.GetNextAssoc(rNextPosition,rKey,rData);
}
