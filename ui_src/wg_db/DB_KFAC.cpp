#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_KFAC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_KFAC::CDB_KFAC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_kfac.InitHashTable(HASHSIZEKFAC);
}

CDB_KFAC::~CDB_KFAC()
{

}

void CDB_KFAC::Add(T_KFAC_K Key,T_KFAC_D& rData,CDB_ELEM* pElem)
{
	T_KFAC_D Data;
	BOOL bExist=m_kfac.Lookup(Key, Data);
	m_kfac.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_KFAC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_KFAC_CMD, Key);
	}
}

BOOL CDB_KFAC::Del(T_KFAC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_kfac.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_KFAC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_KFAC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_KFAC::Get(T_KFAC_K Key,T_KFAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_kfac->Lookup(Key, uTemp)) return FALSE;
	}
	return m_kfac.Lookup(Key,rData);
}

int CDB_KFAC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_kfac->GetCount();
	}
	return m_kfac.GetCount();
}

POSITION CDB_KFAC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_kfac->GetStartPosition();
	}
	return m_kfac.GetStartPosition();
}

void CDB_KFAC::GetNext(POSITION& rNextPosition,T_KFAC_K& rKey,T_KFAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_kfac->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_kfac.Lookup(rKey, rData);
		return;
	}
	m_kfac.GetNextAssoc(rNextPosition,rKey,rData);
}
