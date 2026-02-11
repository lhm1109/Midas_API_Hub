#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MCMB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MCMB::CDB_MCMB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mcmb.InitHashTable(HASHSIZEMCMB);
}

CDB_MCMB::~CDB_MCMB()
{

}

void CDB_MCMB::Add(T_MCMB_K Key,T_MCMB_D& rData,CDB_ELEM* pElem)
{
	T_MCMB_D Data;
	BOOL bExist=m_mcmb.Lookup(Key, Data);
	m_mcmb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MCMB_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_MCMB_CMD, Key);
	}
}

BOOL CDB_MCMB::Del(T_MCMB_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mcmb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_MCMB_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_MCMB_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MCMB::Get(T_MCMB_K Key,T_MCMB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mcmb->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mcmb.Lookup(Key,rData);
}

int CDB_MCMB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mcmb->GetCount();
	}
	return m_mcmb.GetCount();
}

POSITION CDB_MCMB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mcmb->GetStartPosition();
	}
	return m_mcmb.GetStartPosition();
}

void CDB_MCMB::GetNext(POSITION& rNextPosition,T_MCMB_K& rKey,T_MCMB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mcmb->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mcmb.Lookup(rKey, rData);
		return;
	}
	m_mcmb.GetNextAssoc(rNextPosition,rKey,rData);
}

