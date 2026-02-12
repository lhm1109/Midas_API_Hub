#include "stdafx.h"
#include "DB_LCOE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LCOE::CDB_LCOE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lcoe.InitHashTable(HASHSIZELCOE);
}

CDB_LCOE::~CDB_LCOE()
{

}

void CDB_LCOE::Add(T_LCOE_D& rData)
{
	m_lcoe.SetAt(1,rData);
}

BOOL CDB_LCOE::Del()
{
	T_LCOE_D Data;
	BOOL bExist=m_lcoe.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_lcoe.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_LCOE::Get(T_LCOE_D& rData)
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	*/
	return m_lcoe.Lookup(1,rData);
}

int CDB_LCOE::GetCount()
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	*/
	return m_lcoe.GetCount();
}
