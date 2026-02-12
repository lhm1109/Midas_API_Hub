#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_WNPS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WNPS::CDB_WNPS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_wnps.InitHashTable(HASHSIZEWNPS);
}

CDB_WNPS::~CDB_WNPS()
{

}

void CDB_WNPS::Add(T_WNPS_K Key, T_WNPS_D& rData, CDB_STLD* pStld)
{
	T_WNPS_D Data;
	BOOL bExist = m_wnps.Lookup(Key, Data);
	m_wnps.SetAt(Key, rData);
	if (!bExist)pStld->AddListItem(Key, LT_WNPS_CMD, Key);
}

BOOL CDB_WNPS::Del(T_WNPS_K Key, CDB_STLD* pStld)
{
	BOOL ret = m_wnps.RemoveKey(Key);
	ASSERT(ret);
	if (ret && pStld)VERIFY(pStld->DelListItem(Key, LT_WNPS_CMD, Key));
	return ret;
}


//--------------------------------------------------------------------------
BOOL CDB_WNPS::Get(T_WNPS_K Key, T_WNPS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_wnps.Lookup(Key, rData);
}
int CDB_WNPS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wnps.GetCount();
}

POSITION CDB_WNPS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wnps.GetStartPosition();
}

void CDB_WNPS::GetNext(POSITION& rNextPosition, T_WNPS_K& rKey, T_WNPS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_wnps.GetNextAssoc(rNextPosition, rKey, rData);
}
