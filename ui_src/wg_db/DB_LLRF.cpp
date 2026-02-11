// DB_LLRF.cpp: implementation of the CDB_LLRF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_LLRF.h"

#include "DBDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_LLRF::CDB_LLRF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_llrf.InitHashTable(HASHSIZELLRF);
}

CDB_LLRF::~CDB_LLRF()
{

}


void CDB_LLRF::Add(T_LLRF_D& rData)
{
	m_llrf.SetAt(1, rData);
}

BOOL CDB_LLRF::Del()
{
	T_LLRF_D Data;
	BOOL bExist = m_llrf.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_llrf.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_LLRF::Get(T_LLRF_D& rData)
{
	return m_llrf.Lookup(1, rData);
}

int CDB_LLRF::GetCount()
{
	return m_llrf.GetCount();
}
