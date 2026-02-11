#include "stdafx.h"
#include "DB_RKPV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RKPV::CDB_RKPV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rkpv.InitHashTable(HASHSIZERKPV);
}

CDB_RKPV::~CDB_RKPV()
{

}

void CDB_RKPV::Add(T_RKPV_K Key, T_RKPV_D& rData)
{
	T_RKPV_D Data;
	BOOL bExist = m_rkpv.Lookup(Key, Data);
	m_rkpv.SetAt(Key, rData);
}

BOOL CDB_RKPV::Del(T_RKPV_K Key)
{
	BOOL ret = m_rkpv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RKPV::Get(T_RKPV_K Key, T_RKPV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rkpv.Lookup(Key, rData);
}

int CDB_RKPV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rkpv.GetCount();
}

POSITION CDB_RKPV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rkpv.GetStartPosition();
}

void CDB_RKPV::GetNext(POSITION& rNextPosition, T_RKPV_K& rKey, T_RKPV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rkpv.GetNextAssoc(rNextPosition, rKey, rData);
}

