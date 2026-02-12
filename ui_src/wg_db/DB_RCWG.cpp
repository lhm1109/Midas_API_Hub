#include "stdafx.h"
#include "DB_RCWG.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_RCWG::CDB_RCWG()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_rcwg.InitHashTable(HASHSIZERCWG);
}

CDB_RCWG::~CDB_RCWG()
{

}

void CDB_RCWG::Add(T_RCWG_D& rData)
{
	m_rcwg.SetAt(1,rData);
}

BOOL CDB_RCWG::Del()
{
	T_RCWG_D Data;
	BOOL bExist=m_rcwg.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rcwg.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCWG::Get(T_RCWG_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_rcwg.Lookup(1,rData);
}

int CDB_RCWG::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_rcwg.GetCount();
}
