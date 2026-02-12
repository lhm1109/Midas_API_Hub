#include "stdafx.h"
#include "DB_SRCV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRCV::CDB_SRCV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srcv.InitHashTable(HASHSIZESRCV);
}

CDB_SRCV::~CDB_SRCV()
{

}

void CDB_SRCV::Add(T_SRCV_K Key, T_SRCV_D& rData)
{
	T_SRCV_D Data;
	BOOL bExist = m_srcv.Lookup(Key, Data);
	m_srcv.SetAt(Key, rData);
}

BOOL CDB_SRCV::Del(T_SRCV_K Key)
{
	BOOL ret = m_srcv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRCV::Get(T_SRCV_K Key, T_SRCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_srcv.Lookup(Key, rData);
}

int CDB_SRCV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srcv.GetCount();
}

POSITION CDB_SRCV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srcv.GetStartPosition();
}

void CDB_SRCV::GetNext(POSITION& rNextPosition, T_SRCV_K& rKey, T_SRCV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_srcv.GetNextAssoc(rNextPosition, rKey, rData);
}

