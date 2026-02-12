#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_JSFT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_JSFT::CDB_JSFT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_jsft.InitHashTable(HASHSIZEJSFT);
}

CDB_JSFT::~CDB_JSFT()
{

}

void CDB_JSFT::Add(T_JSFT_K Key,T_JSFT_D& rData,CDB_ELEM* pElem)
{
	T_JSFT_D Data;
	BOOL bExist=m_jsft.Lookup(Key, Data);
	m_jsft.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_JSFT_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_JSFT_CMD, Key);
	}
}

BOOL CDB_JSFT::Del(T_JSFT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_jsft.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_JSFT_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_JSFT_CMD, Key);
	}

	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_JSFT::Get(T_JSFT_K Key,T_JSFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_jsft->Lookup(Key, uTemp)) return FALSE;
	}
	return m_jsft.Lookup(Key,rData);
}

int CDB_JSFT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_jsft->GetCount();
	}
	return m_jsft.GetCount();
}

POSITION CDB_JSFT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_jsft->GetStartPosition();
	}
	return m_jsft.GetStartPosition();
}

void CDB_JSFT::GetNext(POSITION& rNextPosition,T_JSFT_K& rKey,T_JSFT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_jsft->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_jsft.Lookup(rKey, rData);
		return;
	}
	m_jsft.GetNextAssoc(rNextPosition,rKey,rData);
}
