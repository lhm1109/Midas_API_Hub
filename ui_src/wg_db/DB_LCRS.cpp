#include "stdafx.h"
#include "DB_LCRS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LCRS::CDB_LCRS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lcrs.InitHashTable(HASHSIZELCRS);
}

CDB_LCRS::~CDB_LCRS()
{

}

void CDB_LCRS::Add(T_LCRS_D& rData)
{
	m_lcrs.SetAt(1,rData);
}

BOOL CDB_LCRS::Del()
{
	T_LCRS_D Data;
	BOOL bExist=m_lcrs.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_lcrs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_LCRS::Get(T_LCRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_lcrs.Lookup(1,rData);
}

int CDB_LCRS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lcrs.GetCount();
}
