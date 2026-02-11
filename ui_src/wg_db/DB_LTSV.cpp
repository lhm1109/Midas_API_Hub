#include "stdafx.h"
#include "DB_LTSV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LTSV::CDB_LTSV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ltsv.InitHashTable(HASHSIZELTSV);
}

CDB_LTSV::~CDB_LTSV()
{

}

void CDB_LTSV::Add(T_LTSV_K Key,T_LTSV_D& rData)
{
	T_LTSV_D Data;
	BOOL bExist=m_ltsv.Lookup(Key, Data);
	m_ltsv.SetAt(Key,rData);
}

BOOL CDB_LTSV::Del(T_LTSV_K Key)
{
	BOOL ret=m_ltsv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LTSV::Get(T_LTSV_K Key,T_LTSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ltsv.Lookup(Key,rData);
}

int CDB_LTSV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ltsv.GetCount();
}

POSITION CDB_LTSV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ltsv.GetStartPosition();
}

void CDB_LTSV::GetNext(POSITION& rNextPosition,T_LTSV_K& rKey,T_LTSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_ltsv.GetNextAssoc(rNextPosition,rKey,rData);
}
