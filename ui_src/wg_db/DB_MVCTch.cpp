#include "stdafx.h"
#include "DB_MVCTch.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCTch::CDB_MVCTch()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvctch.InitHashTable(HASHSIZEMVCTch);
}

CDB_MVCTch::~CDB_MVCTch()
{

}

void CDB_MVCTch::Add(T_MVCTch_D& rData)
{
	m_mvctch.SetAt(1,rData);
}

BOOL CDB_MVCTch::Del()
{
	T_MVCTch_D Data;
	BOOL bExist=m_mvctch.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvctch.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCTch::Get(T_MVCTch_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvctch.Lookup(1,rData);
}

int CDB_MVCTch::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvctch.GetCount();
}
