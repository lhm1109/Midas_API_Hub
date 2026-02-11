#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PFMC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PFMC::CDB_PFMC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_PFMC.InitHashTable(HASHSIZEPFMC);
}

CDB_PFMC::~CDB_PFMC()
{

}

void CDB_PFMC::Add(T_PFMC_K Key,T_PFMC_D& rData,CDB_ELEM* pElem)
{
	T_PFMC_D Data;
	BOOL bExist=m_PFMC.Lookup(Key, Data);
	m_PFMC.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PFMC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PFMC_CMD, Key);
	}
}

BOOL CDB_PFMC::Del(T_PFMC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_PFMC.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PFMC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PFMC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PFMC::Get(T_PFMC_K Key,T_PFMC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pfmc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_PFMC.Lookup(Key,rData);
}

int CDB_PFMC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfmc->GetCount();
	}
	return m_PFMC.GetCount();
}

POSITION CDB_PFMC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pfmc->GetStartPosition();
	}
	return m_PFMC.GetStartPosition();
}

void CDB_PFMC::GetNext(POSITION& rNextPosition,T_PFMC_K& rKey,T_PFMC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pfmc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_PFMC.Lookup(rKey, rData);
		return;
	}
	m_PFMC.GetNextAssoc(rNextPosition,rKey,rData);
}
