#include "stdafx.h"
#include "DB_HDCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HDCT::CDB_HDCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hdct.InitHashTable(HASHSIZEHDCT);
}

CDB_HDCT::~CDB_HDCT()
{

}

void CDB_HDCT::Add(T_HDCT_D& rData)
{
	m_hdct.SetAt(1,rData);
}

BOOL CDB_HDCT::Del()
{
	T_HDCT_D Data;
	BOOL bExist=m_hdct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_hdct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_HDCT::Get(T_HDCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hdct.Lookup(1,rData);
}

int CDB_HDCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hdct.GetCount();
}
