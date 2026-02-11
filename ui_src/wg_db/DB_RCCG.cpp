#include "stdafx.h"
#include "DB_RCCG.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_RCCG::CDB_RCCG()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_rccg.InitHashTable(HASHSIZERCCG);
}

CDB_RCCG::~CDB_RCCG()
{

}

void CDB_RCCG::Add(T_RCCG_D& rData)
{
	m_rccg.SetAt(1,rData);
}

BOOL CDB_RCCG::Del()
{
	T_RCCG_D Data;
	BOOL bExist=m_rccg.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rccg.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCCG::Get(T_RCCG_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return FALSE;
	return m_rccg.Lookup(1,rData);
}

int CDB_RCCG::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	//  return 0;
	return m_rccg.GetCount();
}
