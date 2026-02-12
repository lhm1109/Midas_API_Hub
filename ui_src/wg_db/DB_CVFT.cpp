#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CVFT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CVFT::CDB_CVFT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cvft.InitHashTable(HASHSIZECVFT);
}

CDB_CVFT::~CDB_CVFT()
{

}

void CDB_CVFT::Add(T_CVFT_K Key,T_CVFT_D& rData,CDB_ELEM* pElem)
{
	T_CVFT_D Data;
	BOOL bExist=m_cvft.Lookup(Key, Data);
	m_cvft.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CVFT_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CVFT_CMD, Key);
	}
}

BOOL CDB_CVFT::Del(T_CVFT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cvft.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CVFT_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CVFT_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CVFT::Get(T_CVFT_K Key,T_CVFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cvft->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cvft.Lookup(Key,rData);
}

int CDB_CVFT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cvft->GetCount();
	}
	return m_cvft.GetCount();
}

POSITION CDB_CVFT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cvft->GetStartPosition();
	}
	return m_cvft.GetStartPosition();
}

void CDB_CVFT::GetNext(POSITION& rNextPosition,T_CVFT_K& rKey,T_CVFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cvft->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cvft.Lookup(rKey, rData);
		return;
	}
	m_cvft.GetNextAssoc(rNextPosition,rKey,rData);
}
