#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CSMS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CSMS::CDB_CSMS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_csms.InitHashTable(HASHSIZECSMS);
}

CDB_CSMS::~CDB_CSMS()
{

}

void CDB_CSMS::Add(T_CSMS_K Key,T_CSMS_D& rData,CDB_ELEM* pElem)
{
	T_CSMS_D Data;
	BOOL bExist=m_csms.Lookup(Key, Data);
	m_csms.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CSMS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CSMS_CMD, Key);
	}
}

BOOL CDB_CSMS::Del(T_CSMS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_csms.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CSMS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CSMS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CSMS::Get(T_CSMS_K Key,T_CSMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_csms->Lookup(Key, uTemp)) return FALSE;
	}
	return m_csms.Lookup(Key,rData);
}

int CDB_CSMS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_csms->GetCount();
	}
	return m_csms.GetCount();
}

POSITION CDB_CSMS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_csms->GetStartPosition();
	}
	return m_csms.GetStartPosition();
}

void CDB_CSMS::GetNext(POSITION& rNextPosition,T_CSMS_K& rKey,T_CSMS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_csms->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_csms.Lookup(rKey, rData);
		return;
	}
	m_csms.GetNextAssoc(rNextPosition,rKey,rData);
}
