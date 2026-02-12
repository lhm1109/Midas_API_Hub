#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MBTP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MBTP::CDB_MBTP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mbtp.InitHashTable(HASHSIZEMBTP);
}

CDB_MBTP::~CDB_MBTP()
{

}

void CDB_MBTP::Add(T_MBTP_K Key,T_MBTP_D& rData,CDB_ELEM* pElem)
{
	T_MBTP_D Data;
	BOOL bExist=m_mbtp.Lookup(Key, Data);
	m_mbtp.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MBTP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_MBTP_CMD, Key);
	}
}

BOOL CDB_MBTP::Del(T_MBTP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mbtp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_MBTP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_MBTP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MBTP::Get(T_MBTP_K Key,T_MBTP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mbtp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mbtp.Lookup(Key,rData);
}

int CDB_MBTP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mbtp->GetCount();
	}
	return m_mbtp.GetCount();
}

POSITION CDB_MBTP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mbtp->GetStartPosition();
	}
	return m_mbtp.GetStartPosition();
}

void CDB_MBTP::GetNext(POSITION& rNextPosition,T_MBTP_K& rKey,T_MBTP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mbtp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mbtp.Lookup(rKey, rData);
		return;
	}
	m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);
}
