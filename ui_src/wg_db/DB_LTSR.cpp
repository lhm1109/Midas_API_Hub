#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_LTSR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LTSR::CDB_LTSR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ltsr.InitHashTable(HASHSIZELTSR);
}

CDB_LTSR::~CDB_LTSR()
{

}

void CDB_LTSR::Add(T_LTSR_K Key,T_LTSR_D& rData,CDB_ELEM* pElem)
{
	T_LTSR_D Data;
	BOOL bExist=m_ltsr.Lookup(Key, Data);
	m_ltsr.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_LTSR_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_LTSR_CMD, Key);
	}
}

BOOL CDB_LTSR::Del(T_LTSR_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ltsr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_LTSR_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_LTSR_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LTSR::Get(T_LTSR_K Key,T_LTSR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ltsr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ltsr.Lookup(Key,rData);
}

int CDB_LTSR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ltsr->GetCount();
	}
	return m_ltsr.GetCount();
}

POSITION CDB_LTSR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ltsr->GetStartPosition();
	}
	return m_ltsr.GetStartPosition();
}

void CDB_LTSR::GetNext(POSITION& rNextPosition,T_LTSR_K& rKey,T_LTSR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ltsr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ltsr.Lookup(rKey, rData);
		return;
	}
	m_ltsr.GetNextAssoc(rNextPosition,rKey,rData);
}
