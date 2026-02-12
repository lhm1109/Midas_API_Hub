#include "stdafx.h"
#include "DB_MVCTbs.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCTbs::CDB_MVCTbs()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvctbs.InitHashTable(HASHSIZEMVCTbs);
}

CDB_MVCTbs::~CDB_MVCTbs()
{

}

void CDB_MVCTbs::Add(T_MVCTbs_D& rData)
{
	m_mvctbs.SetAt(1,rData);
}

BOOL CDB_MVCTbs::Del()
{
	T_MVCTbs_D Data;
	BOOL bExist=m_mvctbs.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvctbs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCTbs::Get(T_MVCTbs_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvctbs.Lookup(1,rData);
}

int CDB_MVCTbs::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvctbs.GetCount();
}
