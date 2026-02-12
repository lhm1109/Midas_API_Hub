#include "stdafx.h"
#include "DB_SBCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SBCT::CDB_SBCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sbct.InitHashTable(HASHSIZESBCT);
}

CDB_SBCT::~CDB_SBCT()
{

}

void CDB_SBCT::Add(T_SBCT_D& rData)
{
	m_sbct.SetAt(1,rData);
}

BOOL CDB_SBCT::Del()
{
	T_SBCT_D Data;
	BOOL bExist=m_sbct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_sbct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SBCT::Get(T_SBCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sbct.Lookup(1,rData);
}

int CDB_SBCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sbct.GetCount();
}

