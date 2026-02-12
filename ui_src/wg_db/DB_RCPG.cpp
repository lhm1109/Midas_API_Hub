#include "stdafx.h"
#include "DB_RCPG.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_RCPG::CDB_RCPG()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_rcpg.InitHashTable(HASHSIZERCPG);
}

CDB_RCPG::~CDB_RCPG()
{

}

void CDB_RCPG::Add(T_RCPG_D& rData)
{
	m_rcpg.SetAt(1,rData);
}

BOOL CDB_RCPG::Del()
{
	T_RCPG_D Data;
	BOOL bExist=m_rcpg.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rcpg.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCPG::Get(T_RCPG_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_rcpg.Lookup(1,rData);
}

int CDB_RCPG::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_rcpg.GetCount();
}
