#include "stdafx.h"
#include "DB_DOHV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DOHV::CDB_DOHV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dohv.InitHashTable(HASHSIZEDOHV);
}

CDB_DOHV::~CDB_DOHV()
{

}

void CDB_DOHV::Add(T_DOHV_K Key,T_DOHV_D& rData)
{
	T_DOHV_D Data;
	BOOL bExist=m_dohv.Lookup(Key, Data);
	m_dohv.SetAt(Key,rData);
}

BOOL CDB_DOHV::Del(T_DOHV_K Key)
{
	BOOL ret=m_dohv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DOHV::Get(T_DOHV_K Key,T_DOHV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dohv.Lookup(Key,rData);
}

int CDB_DOHV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dohv.GetCount();
}

POSITION CDB_DOHV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dohv.GetStartPosition();
}

void CDB_DOHV::GetNext(POSITION& rNextPosition,T_DOHV_K& rKey,T_DOHV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dohv.GetNextAssoc(rNextPosition,rKey,rData);
}
