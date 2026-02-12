#include "stdafx.h"
#include "DB_CGDV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGDV::CDB_CGDV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgdv.InitHashTable(HASHSIZECGDV);
}

CDB_CGDV::~CDB_CGDV()
{

}

void CDB_CGDV::Add(T_CGDV_K Key,T_CGDV_D& rData)
{
	T_CGDV_D Data;
	BOOL bExist=m_cgdv.Lookup(Key, Data);
	m_cgdv.SetAt(Key,rData);
}

BOOL CDB_CGDV::Del(T_CGDV_K Key)
{
	BOOL ret=m_cgdv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGDV::Get(T_CGDV_K Key,T_CGDV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgdv.Lookup(Key,rData);
}

int CDB_CGDV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgdv.GetCount();
}

POSITION CDB_CGDV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgdv.GetStartPosition();
}

void CDB_CGDV::GetNext(POSITION& rNextPosition,T_CGDV_K& rKey,T_CGDV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgdv.GetNextAssoc(rNextPosition,rKey,rData);
}
