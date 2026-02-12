#include "stdafx.h"
//#include "DB_ELEM.h"
#include "DB_BCCD.h"

#include "DBDoc.h"

CDB_BCCD::CDB_BCCD()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_bccd.InitHashTable(HASHSIZEBCCD);
}

CDB_BCCD::~CDB_BCCD()
{

}

void CDB_BCCD::Add(T_BCCD_K Key, T_BCCD_D& rData)
{
	T_BCCD_D data;
	m_bccd.SetAt(Key, rData);
	if (Key == m_nStartNum)
	{
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))
				break;
		}
	}
	if (Key > m_nLastNum)
		m_nLastNum = Key;
}

BOOL CDB_BCCD::Del(T_BCCD_K Key)
{
	BOOL ret = m_bccd.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_BCCD_K key;
			T_BCCD_D data;
			if (Get(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = GetStart();
				while (pos != NULL)
				{
					GetNext(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
	}
	return ret;
}

BOOL CDB_BCCD::Get(T_BCCD_K Key, T_BCCD_D& rData)
{
	return m_bccd.Lookup(Key, rData);
}

int CDB_BCCD::GetCount()
{
	return m_bccd.GetCount();
}

POSITION CDB_BCCD::GetStart()
{
	return m_bccd.GetStartPosition();
}

void CDB_BCCD::GetNext(POSITION& rNextPosition, T_BCCD_K& rKey, T_BCCD_D& rData)
{
	m_bccd.GetNextAssoc(rNextPosition, rKey, rData);
}
