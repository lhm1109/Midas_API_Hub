#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MRFT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MRFT::CDB_MRFT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mrft.InitHashTable(HASHSIZEMRFT);
}

CDB_MRFT::~CDB_MRFT()
{

}

void CDB_MRFT::Add(T_MRFT_K Key,T_MRFT_D& rData,CDB_ELEM* pElem)
{
	T_MRFT_D Data;
	BOOL bExist=m_mrft.Lookup(Key, Data);
	m_mrft.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MRFT_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_MRFT_CMD, Key);
	}
}

BOOL CDB_MRFT::Del(T_MRFT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mrft.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_MRFT_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_MRFT_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MRFT::Get(T_MRFT_K Key,T_MRFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mrft->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mrft.Lookup(Key,rData);
}

int CDB_MRFT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mrft->GetCount();
	}
	return m_mrft.GetCount();
}

POSITION CDB_MRFT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mrft->GetStartPosition();
	}
	return m_mrft.GetStartPosition();
}

void CDB_MRFT::GetNext(POSITION& rNextPosition,T_MRFT_K& rKey,T_MRFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mrft->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mrft.Lookup(rKey, rData);
		return;
	}
	m_mrft.GetNextAssoc(rNextPosition,rKey,rData);
}

