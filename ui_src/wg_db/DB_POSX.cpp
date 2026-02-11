#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_POSX.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POSX::CDB_POSX()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_posx.InitHashTable(HASHSIZEPOSX);
}

CDB_POSX::~CDB_POSX()
{

}

void CDB_POSX::Add(T_POSX_K Key,T_POSX_D& rData,CDB_ELEM* pElem)
{
	T_POSX_D Data;
	BOOL bExist=m_posx.Lookup(Key, Data);
	m_posx.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_POSX_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_POSX_CMD, Key);
	}
}

BOOL CDB_POSX::Del(T_POSX_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_posx.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_POSX_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_POSX_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_POSX::Get(T_POSX_K Key,T_POSX_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_posx->Lookup(Key, uTemp)) return FALSE;
	}
	return m_posx.Lookup(Key,rData);
}

int CDB_POSX::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_posx->GetCount();
	}
	return m_posx.GetCount();
}

POSITION CDB_POSX::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_posx->GetStartPosition();
	}
	return m_posx.GetStartPosition();
}

void CDB_POSX::GetNext(POSITION& rNextPosition,T_POSX_K& rKey,T_POSX_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_posx->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_posx.Lookup(rKey, rData);
		return;
	}
	m_posx.GetNextAssoc(rNextPosition,rKey,rData);
}

