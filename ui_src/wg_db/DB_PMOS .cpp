#include "stdafx.h"
#include "DB_PMOS.h"

#include "DBDoc.h"

CDB_PMOS::CDB_PMOS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pmos.InitHashTable(HASHSIZEPMOS);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_PMOS::~CDB_PMOS()
{
}

void CDB_PMOS::Add(T_PMOS_K Key, T_PMOS_D& rData)
{
	T_PMOS_D Data;
	BOOL bExist = m_pmos.Lookup(Key, Data);

	m_pmos.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_PMOS_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_pmos.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_PMOS::Del(T_PMOS_K Key)
{
	T_PMOS_D Data;
	BOOL bExist = m_pmos.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_pmos.RemoveKey(Key);
 		ASSERT(ret);

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_PMOS_K key;
			T_PMOS_D data;
			if (m_pmos.Lookup(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = m_pmos.GetStartPosition();
				while (pos != NULL)
				{
					m_pmos.GetNextAssoc(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

BOOL CDB_PMOS::Get(T_PMOS_K Key, T_PMOS_D& rData)
{
	return m_pmos.Lookup(Key, rData);
}

int CDB_PMOS::GetCount()
{
	return m_pmos.GetCount();
}

POSITION CDB_PMOS::GetStart()
{
	return m_pmos.GetStartPosition();
}

void CDB_PMOS::GetNext(POSITION& rNextPosition, T_PMOS_K& rKey, T_PMOS_D& rData)
{
	m_pmos.GetNextAssoc(rNextPosition, rKey, rData);
}
