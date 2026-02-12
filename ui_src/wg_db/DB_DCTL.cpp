#include "stdafx.h"
#include "DB_DCTL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCTL::CDB_DCTL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dctl.InitHashTable(HASHSIZEDCTL);
}

CDB_DCTL::~CDB_DCTL()
{

}

void CDB_DCTL::Add(T_DCTL_D& rData)
{
	m_dctl.SetAt(1,rData);
}

BOOL CDB_DCTL::Del()
{
	T_DCTL_D Data;
	BOOL bExist=m_dctl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dctl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DCTL::Get(T_DCTL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dctl.Lookup(1,rData);
}

int CDB_DCTL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dctl.GetCount();
}
