#include "stdafx.h"
#include "DB_BLDC.h"

//#include "DBDoc.h"
//#include "StagCtrl.h"

CDB_BLDC::CDB_BLDC()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	m_bldc.InitHashTable(HASHSIZEBLDC);
}

CDB_BLDC::~CDB_BLDC()
{

}

void CDB_BLDC::Add(T_BLDC_D& rData)
{
	m_bldc.SetAt(1,rData);
}

BOOL CDB_BLDC::Del()
{
	T_BLDC_D Data;
	BOOL bExist=m_bldc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_bldc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
/*
BOOL CDB_BLDC::Get(T_BLDC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_bldc.Lookup(1,rData);
}

int CDB_BLDC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_bldc.GetCount();
}
*/