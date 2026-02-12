#include "stdafx.h"
#include "DB_CGFV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGFV::CDB_CGFV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgfv.InitHashTable(HASHSIZECGFV);
}

CDB_CGFV::~CDB_CGFV()
{

}

void CDB_CGFV::Add(T_CGFV_K Key, T_CGFV_D& rData)
{
	T_CGFV_D Data;
	BOOL bExist = m_cgfv.Lookup(Key, Data);
	m_cgfv.SetAt(Key, rData);
}

BOOL CDB_CGFV::Del(T_CGFV_K Key)
{
	BOOL ret = m_cgfv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGFV::Get(T_CGFV_K Key, T_CGFV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgfv.Lookup(Key, rData);
}

int CDB_CGFV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgfv.GetCount();
}

POSITION CDB_CGFV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgfv.GetStartPosition();
}

void CDB_CGFV::GetNext(POSITION& rNextPosition, T_CGFV_K& rKey, T_CGFV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgfv.GetNextAssoc(rNextPosition, rKey, rData);
}
