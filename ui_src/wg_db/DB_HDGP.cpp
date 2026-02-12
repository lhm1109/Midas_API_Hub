#include "stdafx.h"
#include "DB_HDGP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HDGP::CDB_HDGP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hdgp.InitHashTable(HASHSIZEHDGP);
}

CDB_HDGP::~CDB_HDGP()
{

}

void CDB_HDGP::Add(T_HDGP_D& rData)
{
	m_hdgp.SetAt(1,rData);
}

BOOL CDB_HDGP::Del()
{
	T_HDGP_D Data;
	BOOL bExist=m_hdgp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_hdgp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_HDGP::Get(T_HDGP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hdgp.Lookup(1,rData);
}

int CDB_HDGP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hdgp.GetCount();
}
