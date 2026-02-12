#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CWRC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CWRC::CDB_CWRC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cwrc.InitHashTable(HASHSIZECWRC);
}

CDB_CWRC::~CDB_CWRC()
{

}

void CDB_CWRC::Add(T_CWRC_K Key,T_CWRC_D& rData,CDB_ELEM* pElem)
{
	T_CWRC_D Data;
	BOOL bExist=m_cwrc.Lookup(Key, Data);
	m_cwrc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CWRC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CWRC_CMD, Key);
	}
}

BOOL CDB_CWRC::Del(T_CWRC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cwrc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CWRC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CWRC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CWRC::Get(T_CWRC_K Key,T_CWRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cwrc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cwrc.Lookup(Key,rData);
}

int CDB_CWRC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cwrc->GetCount();
	}
	return m_cwrc.GetCount();
}

POSITION CDB_CWRC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cwrc->GetStartPosition();
	}
	return m_cwrc.GetStartPosition();
}

void CDB_CWRC::GetNext(POSITION& rNextPosition,T_CWRC_K& rKey,T_CWRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cwrc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cwrc.Lookup(rKey, rData);
		return;
	}
	m_cwrc.GetNextAssoc(rNextPosition,rKey,rData);
}

