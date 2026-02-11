#include "stdafx.h"
#include "DB_SMCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SMCT::CDB_SMCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_smct.InitHashTable(HASHSIZESMCT);
}

CDB_SMCT::~CDB_SMCT()
{

}

void CDB_SMCT::Add(T_SMCT_D& rData)
{
	m_smct.SetAt(1,rData);
}

BOOL CDB_SMCT::Del()
{
	T_SMCT_D Data;
	BOOL bExist=m_smct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_smct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SMCT::Get(T_SMCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_smct.Lookup(1,rData);
}

int CDB_SMCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_smct.GetCount();
}
