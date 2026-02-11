#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_POSC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POSC::CDB_POSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_posc.InitHashTable(HASHSIZEPOSC);
}

CDB_POSC::~CDB_POSC()
{

}

void CDB_POSC::Add(T_POSC_K Key,T_POSC_D& rData,CDB_ELEM* pElem)
{
	T_POSC_D Data;
	BOOL bExist=m_posc.Lookup(Key, Data);
	m_posc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_POSC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_POSC_CMD, Key);
	}
}

BOOL CDB_POSC::Del(T_POSC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_posc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_POSC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_POSC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_POSC::Get(T_POSC_K Key,T_POSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_posc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_posc.Lookup(Key,rData);
}

int CDB_POSC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_posc->GetCount();
	}
	return m_posc.GetCount();
}

POSITION CDB_POSC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_posc->GetStartPosition();
	}
	return m_posc.GetStartPosition();
}

void CDB_POSC::GetNext(POSITION& rNextPosition,T_POSC_K& rKey,T_POSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_posc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_posc.Lookup(rKey, rData);
		return;
	}
	m_posc.GetNextAssoc(rNextPosition,rKey,rData);
}

