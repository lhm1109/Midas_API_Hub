#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DORC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DORC::CDB_DORC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dorc.InitHashTable(HASHSIZEDORC);
}

CDB_DORC::~CDB_DORC()
{

}

void CDB_DORC::Add(T_DORC_K Key,T_DORC_D& rData,CDB_ELEM* pElem)
{
	T_DORC_D Data;
	BOOL bExist=m_dorc.Lookup(Key, Data);
	m_dorc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DORC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DORC_CMD, Key);
	}
}

BOOL CDB_DORC::Del(T_DORC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dorc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DORC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DORC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DORC::Get(T_DORC_K Key,T_DORC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_dorc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_dorc.Lookup(Key,rData);
}

int CDB_DORC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dorc->GetCount();
	}
	return m_dorc.GetCount();
}

POSITION CDB_DORC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_dorc->GetStartPosition();
	}
	return m_dorc.GetStartPosition();
}

void CDB_DORC::GetNext(POSITION& rNextPosition,T_DORC_K& rKey,T_DORC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_dorc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_dorc.Lookup(rKey, rData);
		return;
	}
	m_dorc.GetNextAssoc(rNextPosition,rKey,rData);
}

