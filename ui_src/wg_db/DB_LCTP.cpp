#include "stdafx.h"
#include "DB_LCTP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LCTP::CDB_LCTP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lctp.InitHashTable(HASHSIZELCTP);
}

CDB_LCTP::~CDB_LCTP()
{

}

void CDB_LCTP::Add(T_LCTP_D& rData)
{
	m_lctp.SetAt(1,rData);
}

BOOL CDB_LCTP::Del()
{
	T_LCTP_D Data;
	BOOL bExist=m_lctp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_lctp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_LCTP::Get(T_LCTP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_lctp.Lookup(1,rData);
}

int CDB_LCTP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lctp.GetCount();
}
