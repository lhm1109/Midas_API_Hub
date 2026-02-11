#include "stdafx.h"
#include "DB_MVCTtr.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCTtr::CDB_MVCTtr()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvcttr.InitHashTable(HASHSIZEMVCTtr);
}

CDB_MVCTtr::~CDB_MVCTtr()
{

}

void CDB_MVCTtr::Add(T_MVCTtr_D& rData)
{
	m_mvcttr.SetAt(1,rData);
}

BOOL CDB_MVCTtr::Del()
{
	T_MVCTtr_D Data;
	BOOL bExist=m_mvcttr.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvcttr.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCTtr::Get(T_MVCTtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvcttr.Lookup(1,rData);
}

int CDB_MVCTtr::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvcttr.GetCount();
}
