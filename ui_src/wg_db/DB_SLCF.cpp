#include "stdafx.h"
#include "DB_SLCF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLCF::CDB_SLCF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_slcf.InitHashTable(HASHSIZESLCF);
}

CDB_SLCF::~CDB_SLCF()
{

}

void CDB_SLCF::Add(T_SLCF_D& rData)
{
	m_slcf.SetAt(1,rData);
}

BOOL CDB_SLCF::Del()
{
	T_SLCF_D Data;
	BOOL bExist=m_slcf.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_slcf.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SLCF::Get(T_SLCF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_slcf.Lookup(1,rData);
}

int CDB_SLCF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_slcf.GetCount();
}
