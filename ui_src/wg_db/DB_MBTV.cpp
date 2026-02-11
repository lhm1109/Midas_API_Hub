#include "stdafx.h"
#include "DB_MBTV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MBTV::CDB_MBTV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mbtv.InitHashTable(HASHSIZEMBTV);
}

CDB_MBTV::~CDB_MBTV()
{

}

void CDB_MBTV::Add(T_MBTV_K Key,T_MBTV_D& rData)
{
	T_MBTV_D Data;
	BOOL bExist=m_mbtv.Lookup(Key, Data);
	m_mbtv.SetAt(Key,rData);
}

BOOL CDB_MBTV::Del(T_MBTV_K Key)
{
	BOOL ret=m_mbtv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MBTV::Get(T_MBTV_K Key,T_MBTV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mbtv.Lookup(Key,rData);
}

int CDB_MBTV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mbtv.GetCount();
}

POSITION CDB_MBTV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mbtv.GetStartPosition();
}

void CDB_MBTV::GetNext(POSITION& rNextPosition,T_MBTV_K& rKey,T_MBTV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mbtv.GetNextAssoc(rNextPosition,rKey,rData);
}
