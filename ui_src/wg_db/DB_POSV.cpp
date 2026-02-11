#include "stdafx.h"
#include "stdafx.h"
#include "DB_POSV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POSV::CDB_POSV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_posv.InitHashTable(HASHSIZEPOSV);
}

CDB_POSV::~CDB_POSV()
{

}

void CDB_POSV::Add(T_POSV_K Key, T_POSV_D& rData)
{
	T_POSV_D Data;
	BOOL bExist = m_posv.Lookup(Key, Data);
	m_posv.SetAt(Key, rData);
}

BOOL CDB_POSV::Del(T_POSV_K Key)
{
	BOOL ret = m_posv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_POSV::Get(T_POSV_K Key, T_POSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_posv.Lookup(Key, rData);
}

int CDB_POSV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_posv.GetCount();
}

POSITION CDB_POSV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_posv.GetStartPosition();
}

void CDB_POSV::GetNext(POSITION& rNextPosition, T_POSV_K& rKey, T_POSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_posv.GetNextAssoc(rNextPosition, rKey, rData);
}

