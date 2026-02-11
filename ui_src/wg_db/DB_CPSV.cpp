#include "stdafx.h"
#include "DB_CPSV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPSV::CDB_CPSV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpsv.InitHashTable(HASHSIZECPSV);
}

CDB_CPSV::~CDB_CPSV()
{

}

void CDB_CPSV::Add(T_CPSV_K Key,T_CPSV_D& rData)
{
	T_CPSV_D Data;
	BOOL bExist=m_cpsv.Lookup(Key, Data);
	m_cpsv.SetAt(Key,rData);
}

BOOL CDB_CPSV::Del(T_CPSV_K Key)
{
	BOOL ret=m_cpsv.RemoveKey(Key);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPSV::Get(T_CPSV_K Key,T_CPSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpsv.Lookup(Key,rData);
}

int CDB_CPSV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpsv.GetCount();
}

POSITION CDB_CPSV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpsv.GetStartPosition();
}

void CDB_CPSV::GetNext(POSITION& rNextPosition,T_CPSV_K& rKey,T_CPSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpsv.GetNextAssoc(rNextPosition,rKey,rData);
}