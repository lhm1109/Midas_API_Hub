#include "stdafx.h"
#include "DB_SRRV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRRV::CDB_SRRV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srrv.InitHashTable(HASHSIZESRRV);
}

CDB_SRRV::~CDB_SRRV()
{

}

void CDB_SRRV::Add(T_SRRV_K Key, T_SRRV_D& rData)
{
	T_SRRV_D Data;
	BOOL bExist = m_srrv.Lookup(Key, Data);
	m_srrv.SetAt(Key, rData);
}

BOOL CDB_SRRV::Del(T_SRRV_K Key)
{
	BOOL ret = m_srrv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRRV::Get(T_SRRV_K Key, T_SRRV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_srrv.Lookup(Key, rData);
}

int CDB_SRRV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srrv.GetCount();
}

POSITION CDB_SRRV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srrv.GetStartPosition();
}

void CDB_SRRV::GetNext(POSITION& rNextPosition, T_SRRV_K& rKey, T_SRRV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_srrv.GetNextAssoc(rNextPosition, rKey, rData);
}

