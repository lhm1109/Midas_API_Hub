#include "stdafx.h"
#include "DB_MVCTjp.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCTjp::CDB_MVCTjp()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvctjp.InitHashTable(HASHSIZEMVCTjp);
}

CDB_MVCTjp::~CDB_MVCTjp()
{

}

void CDB_MVCTjp::Add(T_MVCTjp_D& rData)
{
	m_mvctjp.SetAt(1,rData);
}

BOOL CDB_MVCTjp::Del()
{
	T_MVCTjp_D Data;
	BOOL bExist=m_mvctjp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvctjp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCTjp::Get(T_MVCTjp_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvctjp.Lookup(1,rData);
}

int CDB_MVCTjp::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvctjp.GetCount();
}
