#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_OCDF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_OCDF::CDB_OCDF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ocdf.InitHashTable(HASHSIZEOCDF);
}

CDB_OCDF::~CDB_OCDF()
{

}

void CDB_OCDF::Add(T_OCDF_K Key,T_OCDF_D& rData,CDB_ELEM* pElem)
{
	T_OCDF_D Data;
	BOOL bExist=m_ocdf.Lookup(Key, Data);
	m_ocdf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_OCDF_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_OCDF_CMD, Key);
	}
}

BOOL CDB_OCDF::Del(T_OCDF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ocdf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_OCDF_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_OCDF_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_OCDF::Get(T_OCDF_K Key,T_OCDF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ocdf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ocdf.Lookup(Key,rData);
}

int CDB_OCDF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ocdf->GetCount();
	}
	return m_ocdf.GetCount();
}

POSITION CDB_OCDF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ocdf->GetStartPosition();
	}
	return m_ocdf.GetStartPosition();
}

void CDB_OCDF::GetNext(POSITION& rNextPosition,T_OCDF_K& rKey,T_OCDF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ocdf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ocdf.Lookup(rKey, rData);
		return;
	}
	m_ocdf.GetNextAssoc(rNextPosition,rKey,rData);
}

