#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_AHTR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_AHTR::CDB_AHTR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ahtr.InitHashTable(HASHSIZEAHTR);
}

CDB_AHTR::~CDB_AHTR()
{

}

void CDB_AHTR::Add(T_AHTR_K Key,T_AHTR_D& rData,CDB_ELEM* pElem)
{
	T_AHTR_D Data;
	BOOL bExist=m_ahtr.Lookup(Key, Data);
	m_ahtr.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_AHTR_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_AHTR_CMD, Key);
	}
}

BOOL CDB_AHTR::Del(T_AHTR_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ahtr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_AHTR_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_AHTR_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_AHTR::Get(T_AHTR_K Key,T_AHTR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ahtr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ahtr.Lookup(Key,rData);
}

int CDB_AHTR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ahtr->GetCount();
	}
	return m_ahtr.GetCount();
}

POSITION CDB_AHTR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ahtr->GetStartPosition();
	}
	return m_ahtr.GetStartPosition();
}

void CDB_AHTR::GetNext(POSITION& rNextPosition,T_AHTR_K& rKey,T_AHTR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ahtr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ahtr.Lookup(rKey, rData);
		return;
	}
	m_ahtr.GetNextAssoc(rNextPosition,rKey,rData);
}
