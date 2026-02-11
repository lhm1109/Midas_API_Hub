#include "stdafx.h"
#include "DB_MATL.h"

#include "DB_PDCM.h"

CDB_PDCM::CDB_PDCM()
{
	m_pdcm.InitHashTable(HASHSIZEPDCM);
}

CDB_PDCM::~CDB_PDCM()
{

}

void CDB_PDCM::Add(T_PDCM_K Key, T_PDCM_D& rData, CDB_MATL* pMatl)
{
	T_PDCM_D Data;
	BOOL bExist = m_pdcm.Lookup(Key, Data);
	m_pdcm.SetAt(Key, rData);

	if ( !bExist && pMatl != NULL )
	{
		pMatl->AddListItem(Key, LT_PDCM_CMD, Key);
	}
}

BOOL CDB_PDCM::Del(T_PDCM_K Key, CDB_MATL* pMatl)
{
	T_PDCM_D Data;
	BOOL bExist = m_pdcm.Lookup(Key, Data);
	if ( bExist )
	{
		BOOL ret = m_pdcm.RemoveKey(Key);
		ASSERT(ret);
		if ( ret )
		{
			if ( pMatl != NULL )VERIFY(pMatl->DelListItem(Key, LT_PDCM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

BOOL CDB_PDCM::Get(T_PDCM_K Key, T_PDCM_D& rData)
{
	return m_pdcm.Lookup(Key, rData);
}

int CDB_PDCM::GetCount()
{
	return m_pdcm.GetCount();
}

POSITION CDB_PDCM::GetStart()
{
	return m_pdcm.GetStartPosition();
}

void CDB_PDCM::GetNext(POSITION& rNextPosition, T_PDCM_K& rKey, T_PDCM_D& rData)
{
	m_pdcm.GetNextAssoc(rNextPosition, rKey, rData);
}
