#include "stdafx.h"
#include "DB_CPCV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPCV::CDB_CPCV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpcv.InitHashTable(HASHSIZECPCV);
}

CDB_CPCV::~CDB_CPCV()
{

}

void CDB_CPCV::Add(T_CPCV_K Key,T_CPCV_D& rData)
{
	T_CPCV_D Data;
	BOOL bExist=m_cpcv.Lookup(Key, Data);
	m_cpcv.SetAt(Key,rData);
}

BOOL CDB_CPCV::Del(T_CPCV_K Key)
{
	BOOL ret=m_cpcv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPCV::Get(T_CPCV_K Key,T_CPCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpcv.Lookup(Key,rData);
}

int CDB_CPCV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpcv.GetCount();
}

POSITION CDB_CPCV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpcv.GetStartPosition();
}

void CDB_CPCV::GetNext(POSITION& rNextPosition,T_CPCV_K& rKey,T_CPCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpcv.GetNextAssoc(rNextPosition,rKey,rData);
}
