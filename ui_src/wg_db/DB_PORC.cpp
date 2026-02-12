#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PORC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PORC::CDB_PORC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_porc.InitHashTable(HASHSIZEPORC);
}

CDB_PORC::~CDB_PORC()
{

}

void CDB_PORC::Add(T_PORC_K Key,T_PORC_D& rData,CDB_ELEM* pElem)
{
	T_PORC_D Data;
	BOOL bExist=m_porc.Lookup(Key, Data);
	m_porc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PORC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PORC_CMD, Key);
	}
}

BOOL CDB_PORC::Del(T_PORC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_porc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PORC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PORC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PORC::Get(T_PORC_K Key,T_PORC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_porc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_porc.Lookup(Key,rData);
}

int CDB_PORC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_porc->GetCount();
	}
	return m_porc.GetCount();
}

POSITION CDB_PORC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_porc->GetStartPosition();
	}
	return m_porc.GetStartPosition();
}

void CDB_PORC::GetNext(POSITION& rNextPosition,T_PORC_K& rKey,T_PORC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_porc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_porc.Lookup(rKey, rData);
		return;
	}
	m_porc.GetNextAssoc(rNextPosition,rKey,rData);
}

