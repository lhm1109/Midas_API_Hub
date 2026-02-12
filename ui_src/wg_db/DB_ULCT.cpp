#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ULCT.h"

#include "DBDoc.h"

CDB_ULCT::CDB_ULCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ulct.InitHashTable(HASHSIZEULCT);
}

CDB_ULCT::~CDB_ULCT()
{

}

void CDB_ULCT::Add(T_ULCT_K Key, T_ULCT_D& rData, CDB_ELEM* pElem)
{
	T_ULCT_D Data;
	BOOL bExist = m_ulct.Lookup(Key, Data);
	m_ulct.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_ULCT_CMD, Key);
}

BOOL CDB_ULCT::Del(T_ULCT_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_ulct.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_ULCT_CMD, Key));
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ULCT::Get(T_ULCT_K Key, T_ULCT_D& rData)
{
	return m_ulct.Lookup(Key, rData);
}

int CDB_ULCT::GetCount()
{
	return m_ulct.GetCount();
}

POSITION CDB_ULCT::GetStart()
{
	return m_ulct.GetStartPosition();
}

void CDB_ULCT::GetNext(POSITION& rNextPosition, T_ULCT_K& rKey, T_ULCT_D& rData)
{
	m_ulct.GetNextAssoc(rNextPosition, rKey, rData);
}
