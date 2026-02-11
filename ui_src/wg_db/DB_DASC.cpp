#include "stdafx.h"
#include "DB_DASC.h"

CDB_DASC::CDB_DASC()
{
	m_dasc.InitHashTable(HASHSIZEDASC);
}

CDB_DASC::~CDB_DASC()
{

}

void CDB_DASC::Add(T_DASC_D& rData)
{
	m_dasc.SetAt(1,rData);
}

BOOL CDB_DASC::Del()
{
	T_DASC_D Data;
	BOOL bExist=m_dasc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dasc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

