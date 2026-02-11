#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDDP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDDP::CDB_SDDP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sddp.InitHashTable(HASHSIZESDDP);
}

CDB_SDDP::~CDB_SDDP()
{

}

void CDB_SDDP::Add(T_SDDP_K Key,T_SDDP_D& rData,CDB_ELEM* pElem)
{
	T_SDDP_D Data;
	BOOL bExist=m_sddp.Lookup(Key, Data);
	m_sddp.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDDP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDDP_CMD, Key);
	}
}

BOOL CDB_SDDP::Del(T_SDDP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sddp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDDP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDDP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDDP::Get(T_SDDP_K Key,T_SDDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sddp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sddp.Lookup(Key,rData);
}

int CDB_SDDP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sddp->GetCount();
	}
	return m_sddp.GetCount();
}

POSITION CDB_SDDP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sddp->GetStartPosition();
	}
	return m_sddp.GetStartPosition();
}

void CDB_SDDP::GetNext(POSITION& rNextPosition,T_SDDP_K& rKey,T_SDDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sddp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sddp.Lookup(rKey, rData);
		return;
	}
	m_sddp.GetNextAssoc(rNextPosition,rKey,rData);
}

