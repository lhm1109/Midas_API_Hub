#include "stdafx.h"
#include "DB_MVCTfr.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCTfr::CDB_MVCTfr()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvctfr.InitHashTable(HASHSIZEMVCTfr);
}

CDB_MVCTfr::~CDB_MVCTfr()
{

}

void CDB_MVCTfr::Add(T_MVCTfr_D& rData)
{
	m_mvctfr.SetAt(1,rData);
}

BOOL CDB_MVCTfr::Del()
{
	T_MVCTfr_D Data;
	BOOL bExist=m_mvctfr.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvctfr.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCTfr::Get(T_MVCTfr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvctfr.Lookup(1,rData);
}

int CDB_MVCTfr::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvctfr.GetCount();
}
