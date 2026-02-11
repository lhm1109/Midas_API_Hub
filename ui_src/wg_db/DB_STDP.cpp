#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_STDP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STDP::CDB_STDP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stdp.InitHashTable(HASHSIZESTDP);
}

CDB_STDP::~CDB_STDP()
{

}

void CDB_STDP::Add(T_STDP_K Key,T_STDP_D& rData,CDB_ELEM* pElem)
{
	T_STDP_D Data;
	BOOL bExist=m_stdp.Lookup(Key, Data);
	m_stdp.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_STDP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_STDP_CMD, Key);
	}
}

BOOL CDB_STDP::Del(T_STDP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_stdp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_STDP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_STDP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STDP::Get(T_STDP_K Key,T_STDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_stdp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_stdp.Lookup(Key,rData);
}

int CDB_STDP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stdp->GetCount();
	}
	return m_stdp.GetCount();
}

POSITION CDB_STDP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stdp->GetStartPosition();
	}
	return m_stdp.GetStartPosition();
}

void CDB_STDP::GetNext(POSITION& rNextPosition,T_STDP_K& rKey,T_STDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_stdp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_stdp.Lookup(rKey, rData);
		return;
	}
	m_stdp.GetNextAssoc(rNextPosition,rKey,rData);
}
