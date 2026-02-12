#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CMFT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CMFT::CDB_CMFT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cmft.InitHashTable(HASHSIZECMFT);
}

CDB_CMFT::~CDB_CMFT()
{

}

void CDB_CMFT::Add(T_CMFT_K Key,T_CMFT_D& rData,CDB_ELEM* pElem)
{
	T_CMFT_D Data;
	BOOL bExist=m_cmft.Lookup(Key, Data);
	m_cmft.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CMFT_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CMFT_CMD, Key);
	}
}

BOOL CDB_CMFT::Del(T_CMFT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cmft.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CMFT_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CMFT_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CMFT::Get(T_CMFT_K Key,T_CMFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cmft->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cmft.Lookup(Key,rData);
}

int CDB_CMFT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cmft->GetCount();
	}
	return m_cmft.GetCount();
}

POSITION CDB_CMFT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cmft->GetStartPosition();
	}
	return m_cmft.GetStartPosition();
}

void CDB_CMFT::GetNext(POSITION& rNextPosition,T_CMFT_K& rKey,T_CMFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cmft->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cmft.Lookup(rKey, rData);
		return;
	}
	m_cmft.GetNextAssoc(rNextPosition,rKey,rData);
}

