#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGRV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGRV::CDB_CGRV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgrv.InitHashTable(HASHSIZECGRV);
}

CDB_CGRV::~CDB_CGRV()
{

}

void CDB_CGRV::Add(T_CGRV_K Key,T_CGRV_D& rData)
{
	T_CGRV_D Data;
	BOOL bExist=m_cgrv.Lookup(Key, Data);
	m_cgrv.SetAt(Key,rData);
}

BOOL CDB_CGRV::Del(T_CGRV_K Key)
{
	BOOL ret=m_cgrv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGRV::Get(T_CGRV_K Key,T_CGRV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgrv.Lookup(Key,rData);
}

int CDB_CGRV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgrv.GetCount();
}

POSITION CDB_CGRV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgrv.GetStartPosition();
}

void CDB_CGRV::GetNext(POSITION& rNextPosition,T_CGRV_K& rKey,T_CGRV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgrv.GetNextAssoc(rNextPosition,rKey,rData);
}
