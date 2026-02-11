#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_FMAG.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FMAG::CDB_FMAG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fmag.InitHashTable(HASHSIZEFMAG);
}

CDB_FMAG::~CDB_FMAG()
{

}

void CDB_FMAG::Add(T_FMAG_K Key,T_FMAG_D& rData,CDB_ELEM* pElem)
{
	T_FMAG_D Data;
	BOOL bExist=m_fmag.Lookup(Key, Data);
	m_fmag.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_FMAG_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_FMAG_CMD, Key);
	}
}

BOOL CDB_FMAG::Del(T_FMAG_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_fmag.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_FMAG_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_FMAG_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_FMAG::Get(T_FMAG_K Key,T_FMAG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_fmag->Lookup(Key, uTemp)) return FALSE;
	}
	return m_fmag.Lookup(Key,rData);
}

int CDB_FMAG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_fmag->GetCount();
	}
	return m_fmag.GetCount();
}

POSITION CDB_FMAG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_fmag->GetStartPosition();
	}
	return m_fmag.GetStartPosition();
}

void CDB_FMAG::GetNext(POSITION& rNextPosition,T_FMAG_K& rKey,T_FMAG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_fmag->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_fmag.Lookup(rKey, rData);
		return;
	}
	m_fmag.GetNextAssoc(rNextPosition,rKey,rData);
}
