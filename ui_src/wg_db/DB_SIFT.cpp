// DB_SIFT.cpp: implementation of the CDB_SIFT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_SIFT.h"

#include "DBDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_SIFT::CDB_SIFT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sift.InitHashTable(HASHSIZESIFT);
}

CDB_SIFT::~CDB_SIFT()
{

}


void CDB_SIFT::Add(T_SIFT_D& rData)
{
	m_sift.SetAt(1, rData);
}

BOOL CDB_SIFT::Del()
{
	T_SIFT_D Data;
	BOOL bExist = m_sift.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_sift.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SIFT::Get(T_SIFT_D& rData)
{
	return m_sift.Lookup(1, rData);
}

int CDB_SIFT::GetCount()
{
	return m_sift.GetCount();
}
