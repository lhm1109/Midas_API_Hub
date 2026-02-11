#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_APHI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_APHI::CDB_APHI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_aphi.InitHashTable(HASHSIZEAPHI);
}

CDB_APHI::~CDB_APHI()
{

}

void CDB_APHI::Add(T_APHI_K Key,T_APHI_D& rData,CDB_ELEM* pElem)
{
	T_APHI_D Data;
	BOOL bExist=m_aphi.Lookup(Key, Data);
	m_aphi.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_APHI_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_APHI_CMD, Key);
	}
}

BOOL CDB_APHI::Del(T_APHI_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_aphi.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_APHI_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_APHI_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_APHI::Get(T_APHI_K Key,T_APHI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_aphi->Lookup(Key, uTemp)) return FALSE;
	}
	return m_aphi.Lookup(Key,rData);
}

int CDB_APHI::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_aphi->GetCount();
	}
	return m_aphi.GetCount();
}

POSITION CDB_APHI::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_aphi->GetStartPosition();
	}
	return m_aphi.GetStartPosition();
}

void CDB_APHI::GetNext(POSITION& rNextPosition,T_APHI_K& rKey,T_APHI_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_aphi->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_aphi.Lookup(rKey, rData);
		return;
	}
	m_aphi.GetNextAssoc(rNextPosition,rKey,rData);
}
