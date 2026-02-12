#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RPRC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPRC::CDB_RPRC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rprc.InitHashTable(HASHSIZERPRC);
}

CDB_RPRC::~CDB_RPRC()
{

}

void CDB_RPRC::Add(T_RPRC_K Key,T_RPRC_D& rData,CDB_ELEM* pElem)
{
	T_RPRC_D Data;
	BOOL bExist=m_rprc.Lookup(Key, Data);
	m_rprc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RPRC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RPRC_CMD, Key);
	}
}

BOOL CDB_RPRC::Del(T_RPRC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rprc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RPRC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RPRC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RPRC::Get(T_RPRC_K Key,T_RPRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rprc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rprc.Lookup(Key,rData);
}

int CDB_RPRC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rprc->GetCount();
	}
	return m_rprc.GetCount();
}

POSITION CDB_RPRC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rprc->GetStartPosition();
	}
	return m_rprc.GetStartPosition();
}

void CDB_RPRC::GetNext(POSITION& rNextPosition,T_RPRC_K& rKey,T_RPRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rprc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rprc.Lookup(rKey, rData);
		return;
	}
	m_rprc.GetNextAssoc(rNextPosition,rKey,rData);
}

