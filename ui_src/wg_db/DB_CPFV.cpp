#include "stdafx.h"
#include "DB_CPFV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPFV::CDB_CPFV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpfv.InitHashTable(HASHSIZECPFV);
}

CDB_CPFV::~CDB_CPFV()
{

}

void CDB_CPFV::Add(T_CPFV_K Key,T_CPFV_D& rData)
{
	T_CPFV_D Data;
	BOOL bExist=m_cpfv.Lookup(Key, Data);
	m_cpfv.SetAt(Key,rData);
}

BOOL CDB_CPFV::Del(T_CPFV_K Key)
{
	BOOL ret=m_cpfv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPFV::Get(T_CPFV_K Key,T_CPFV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpfv.Lookup(Key,rData);
}

int CDB_CPFV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpfv.GetCount();
}

POSITION CDB_CPFV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpfv.GetStartPosition();
}

void CDB_CPFV::GetNext(POSITION& rNextPosition,T_CPFV_K& rKey,T_CPFV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpfv.GetNextAssoc(rNextPosition,rKey,rData);
}
