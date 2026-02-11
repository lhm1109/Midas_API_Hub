#include "stdafx.h"
#include "DB_PMIS.h"
#include "DB_PMOS.h"

#include "DBDoc.h"

CDB_PMIS::CDB_PMIS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pmis.InitHashTable(HASHSIZEPMIS);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_PMIS::~CDB_PMIS()
{
}

void CDB_PMIS::Add(T_PMIS_K Key, T_PMIS_D& rData)
{
	T_PMIS_D Data;
	BOOL bExist = m_pmis.Lookup(Key, Data);

	m_pmis.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_PMIS_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_pmis.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_PMIS::Del(T_PMIS_K Key)
{
	T_PMIS_D Data;
	BOOL bExist = m_pmis.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_pmis.RemoveKey(Key);
		ASSERT(ret);

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_PMIS_K key;
			T_PMIS_D data;
			if (m_pmis.Lookup(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = m_pmis.GetStartPosition();
				while (pos != NULL)
				{
					m_pmis.GetNextAssoc(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

BOOL CDB_PMIS::Get(T_PMIS_K Key, T_PMIS_D& rData)
{
	return m_pmis.Lookup(Key, rData);
}


int CDB_PMIS::GetCount()
{
	return m_pmis.GetCount();
}

POSITION CDB_PMIS::GetStart()
{
	return m_pmis.GetStartPosition();
}

void CDB_PMIS::GetNext(POSITION& rNextPosition, T_PMIS_K& rKey, T_PMIS_D& rData)
{
	m_pmis.GetNextAssoc(rNextPosition, rKey, rData);
}