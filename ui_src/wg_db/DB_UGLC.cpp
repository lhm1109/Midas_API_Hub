// DB_UGLC.cpp: implementation of the CDB_UGLC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_UGLC.h"

#include "DBDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_UGLC::CDB_UGLC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_uglc.InitHashTable(HASHSIZEUGLC);
}

CDB_UGLC::~CDB_UGLC()
{

}


void CDB_UGLC::Add(T_UGLC_D& rData)
{
	m_uglc.SetAt(1, rData);
}

BOOL CDB_UGLC::Del()
{
	T_UGLC_D Data;
	BOOL bExist = m_uglc.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_uglc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_UGLC::Get(T_UGLC_D& rData)
{
	return m_uglc.Lookup(1, rData);
}

int CDB_UGLC::GetCount()
{
	return m_uglc.GetCount();
}
