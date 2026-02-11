#include "stdafx.h"
#include "DB_RCTI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCTI::CDB_RCTI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcti.InitHashTable(HASHSIZERCTI);
}

CDB_RCTI::~CDB_RCTI()
{

}

void CDB_RCTI::Add(T_RCTI_D& rData)
{
	m_rcti.SetAt(1, rData);
}

BOOL CDB_RCTI::Del()
{
	T_RCTI_D Data;
	BOOL bExist = m_rcti.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_rcti.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCTI::Get(T_RCTI_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rcti.Lookup(1, rData);
}

int CDB_RCTI::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rcti.GetCount();
}
