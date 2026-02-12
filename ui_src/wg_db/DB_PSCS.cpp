#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PSCS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSCS::CDB_PSCS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pscs.InitHashTable(HASHSIZEPSCS);
}

CDB_PSCS::~CDB_PSCS()
{

}

void CDB_PSCS::Add(T_PSCS_K Key,T_PSCS_D& rData,CDB_ELEM* pElem)
{
	T_PSCS_D Data;
	BOOL bExist=m_pscs.Lookup(Key, Data);
	m_pscs.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PSCS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PSCS_CMD, Key);
	}
}

BOOL CDB_PSCS::Del(T_PSCS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_pscs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PSCS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PSCS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PSCS::Get(T_PSCS_K Key,T_PSCS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pscs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_pscs.Lookup(Key,rData);
}

int CDB_PSCS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pscs->GetCount();
	}
	return m_pscs.GetCount();
}

POSITION CDB_PSCS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pscs->GetStartPosition();
	}
	return m_pscs.GetStartPosition();
}

void CDB_PSCS::GetNext(POSITION& rNextPosition,T_PSCS_K& rKey,T_PSCS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pscs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_pscs.Lookup(rKey, rData);
		return;
	}
	m_pscs.GetNextAssoc(rNextPosition,rKey,rData);
}

