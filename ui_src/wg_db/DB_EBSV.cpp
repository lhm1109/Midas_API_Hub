#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_EBSV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EBSV::CDB_EBSV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ebsv.InitHashTable(HASHSIZEEBSV);
}

CDB_EBSV::~CDB_EBSV()
{

}

void CDB_EBSV::Add(T_EBSV_K Key, T_EBSV_D& rData/*, CDB_ELEM* pElem*/)
{
	T_EBSV_D Data;
	BOOL bExist = m_ebsv.Lookup(Key, Data);
	m_ebsv.SetAt(Key, rData);
	//if (!bExist)pElem->AddListItem(Key, LT_EBSV_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
// 	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
// 	{
// 		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_EBSV_CMD, Key);
// 	}
}

BOOL CDB_EBSV::Del(T_EBSV_K Key/*, CDB_ELEM* pElem*/)
{
	BOOL ret = m_ebsv.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		//VERIFY(pElem->DelListItem(Key, LT_EBSV_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
// 		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
// 			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_EBSV_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_EBSV::Get(T_EBSV_K Key, T_EBSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
// 		UINT uTemp;
// 		if (!m_pDoc->m_pStagCtrl->m_ebsv->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ebsv.Lookup(Key, rData);
}

int CDB_EBSV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
// 		return m_pDoc->m_pStagCtrl->m_ebsv->GetCount();
	}
	return m_ebsv.GetCount();
}

POSITION CDB_EBSV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
// 		return m_pDoc->m_pStagCtrl->m_ebsv->GetStartPosition();
	}
	return m_ebsv.GetStartPosition();
}

void CDB_EBSV::GetNext(POSITION& rNextPosition, T_EBSV_K& rKey, T_EBSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
// 		m_pDoc->m_pStagCtrl->m_ebsv->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ebsv.Lookup(rKey, rData);
		return;
	}
	m_ebsv.GetNextAssoc(rNextPosition, rKey, rData);
}

