#include "stdafx.h"
#include "DB_SLDC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLDC::CDB_SLDC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sldc.InitHashTable(HASHSIZESLDC);
}

CDB_SLDC::~CDB_SLDC()
{

}

void CDB_SLDC::Add(T_SLDC_D& rData)
{
	m_sldc.SetAt(1,rData);
}

BOOL CDB_SLDC::Del()
{
	T_SLDC_D Data;
	BOOL bExist=m_sldc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_sldc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SLDC::Get(T_SLDC_D& rData)
{
	return m_sldc.Lookup(1,rData);
}

int CDB_SLDC::GetCount()
{
	return m_sldc.GetCount();
}
