#include "stdafx.h"
#include "DB_CHRP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CHRP::CDB_CHRP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_chrp.InitHashTable(HASHSIZECHRP);
}

CDB_CHRP::~CDB_CHRP()
{

}

void CDB_CHRP::Add(T_CHRP_D& rData)
{
	m_chrp.SetAt(1,rData);
}

BOOL CDB_CHRP::Del()
{
	T_CHRP_D Data;
	BOOL bExist=m_chrp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_chrp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_CHRP::Get(T_CHRP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_chrp.Lookup(1,rData);
}

int CDB_CHRP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_chrp.GetCount();
}
