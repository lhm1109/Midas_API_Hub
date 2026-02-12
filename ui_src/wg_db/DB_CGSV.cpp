#include "stdafx.h"

#include "DB_CGSV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGSV::CDB_CGSV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgsv.InitHashTable(HASHSIZECGSV);
}

CDB_CGSV::~CDB_CGSV()
{

}

void CDB_CGSV::Add(T_CGSV_K Key, T_CGSV_D& rData)
{
	T_CGSV_D Data;
	BOOL bExist = m_cgsv.Lookup(Key, Data);
	m_cgsv.SetAt(Key, rData);
}

BOOL CDB_CGSV::Del(T_CGSV_K Key)
{
	BOOL ret = m_cgsv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGSV::Get(T_CGSV_K Key, T_CGSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgsv.Lookup(Key, rData);
}

int CDB_CGSV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgsv.GetCount();
}

POSITION CDB_CGSV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgsv.GetStartPosition();
}

void CDB_CGSV::GetNext(POSITION& rNextPosition, T_CGSV_K& rKey, T_CGSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgsv.GetNextAssoc(rNextPosition, rKey, rData);
}
