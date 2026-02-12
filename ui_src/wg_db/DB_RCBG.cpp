#include "stdafx.h"
#include "DB_RCBG.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_RCBG::CDB_RCBG()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_rcbg.InitHashTable(HASHSIZERCBG);
}

CDB_RCBG::~CDB_RCBG()
{

}

void CDB_RCBG::Add(T_RCBG_D& rData)
{
	m_rcbg.SetAt(1,rData);
}

BOOL CDB_RCBG::Del()
{
	T_RCBG_D Data;
	BOOL bExist=m_rcbg.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rcbg.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCBG::Get(T_RCBG_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_rcbg.Lookup(1,rData);
}

int CDB_RCBG::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_rcbg.GetCount();
}
