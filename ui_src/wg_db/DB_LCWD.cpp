#include "stdafx.h"
#include "DB_LCWD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LCWD::CDB_LCWD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lcwd.InitHashTable(HASHSIZELCWD);
}

CDB_LCWD::~CDB_LCWD()
{

}

void CDB_LCWD::Add(T_LCWD_D& rData)
{
	m_lcwd.SetAt(1,rData);
}

BOOL CDB_LCWD::Del()
{
	T_LCWD_D Data;
	BOOL bExist=m_lcwd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_lcwd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_LCWD::Get(T_LCWD_D& rData)
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	*/
	return m_lcwd.Lookup(1,rData);
}

int CDB_LCWD::GetCount()
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	*/
	return m_lcwd.GetCount();
}
