#include "stdafx.h"
#include "DB_RCDC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCDC::CDB_RCDC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcdc.InitHashTable(HASHSIZERACD);
}

CDB_RCDC::~CDB_RCDC()
{

}

void CDB_RCDC::Add(T_RCDC_D& rData)
{
	m_rcdc.SetAt(1,rData);
}

BOOL CDB_RCDC::Del()
{
	T_RCDC_D Data;
	BOOL bExist=m_rcdc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rcdc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RCDC::Get(T_RCDC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rcdc.Lookup(1,rData);
}

int CDB_RCDC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rcdc.GetCount();
}
