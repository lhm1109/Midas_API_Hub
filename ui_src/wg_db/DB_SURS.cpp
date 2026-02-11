// DB_SURS.cpp: implementation of the CDB_SURS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_SURS.h"

#include "DBDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_SURS::CDB_SURS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_surs.InitHashTable(HASHSIZESURS);
}

CDB_SURS::~CDB_SURS()
{

}


void CDB_SURS::Add(T_SURS_D& rData)
{
	m_surs.SetAt(1,rData);
}

BOOL CDB_SURS::Del()
{
	T_SURS_D Data;
	BOOL bExist=m_surs.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_surs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SURS::Get(T_SURS_D& rData)
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	*/
	return m_surs.Lookup(1,rData);
}

int CDB_SURS::GetCount()
{
	/*
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	*/
	return m_surs.GetCount();
}
