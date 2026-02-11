#include "stdafx.h"
#include "DB_RPRV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPRV::CDB_RPRV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rprv.InitHashTable(HASHSIZERPRV);
}

CDB_RPRV::~CDB_RPRV()
{

}

void CDB_RPRV::Add(T_RPRV_K Key,T_RPRV_D& rData)
{
	T_RPRV_D Data;
	BOOL bExist=m_rprv.Lookup(Key, Data);
	m_rprv.SetAt(Key,rData);
}

BOOL CDB_RPRV::Del(T_RPRV_K Key)
{
	BOOL ret=m_rprv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RPRV::Get(T_RPRV_K Key,T_RPRV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rprv.Lookup(Key,rData);
}

int CDB_RPRV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rprv.GetCount();
}

POSITION CDB_RPRV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rprv.GetStartPosition();
}

void CDB_RPRV::GetNext(POSITION& rNextPosition,T_RPRV_K& rKey,T_RPRV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_rprv.GetNextAssoc(rNextPosition,rKey,rData);
}

