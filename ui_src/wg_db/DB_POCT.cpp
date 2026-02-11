#include "stdafx.h"
#include "DB_POCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POCT::CDB_POCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_poct.InitHashTable(HASHSIZEPOCT);
}

CDB_POCT::~CDB_POCT()
{

}

void CDB_POCT::Add(T_POCT_D& rData)
{
	m_poct.SetAt(1,rData);
}

BOOL CDB_POCT::Del()
{
	T_POCT_D Data;
	BOOL bExist=m_poct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_poct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_POCT::Get(T_POCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_poct.Lookup(1,rData);
}

int CDB_POCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_poct.GetCount();
}
