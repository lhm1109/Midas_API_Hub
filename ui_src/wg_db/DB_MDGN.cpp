#include "stdafx.h"
#include "DB_MDGN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MDGN::CDB_MDGN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mdgn.InitHashTable(HASHSIZEMDGN);
}

CDB_MDGN::~CDB_MDGN()
{

}

void CDB_MDGN::Add(T_MDGN_D& rData)
{
	m_mdgn.SetAt(1,rData);
}

BOOL CDB_MDGN::Del()
{
	T_MDGN_D Data;
	BOOL bExist=m_mdgn.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mdgn.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MDGN::Get(T_MDGN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mdgn.Lookup(1,rData);
}

int CDB_MDGN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mdgn.GetCount();
}
