#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_POSL.h"

#include "DBDoc.h"

CDB_POSL::CDB_POSL()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_posl.InitHashTable(HASHSIZEPOSL);
}

CDB_POSL::~CDB_POSL()
{

}

void CDB_POSL::Add(T_POSL_K Key, T_POSL_D& rData)
{
	T_POSL_D data;
	m_posl.SetAt(Key, rData);
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

BOOL CDB_POSL::Del(T_POSL_K Key)
{
	BOOL ret = m_posl.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_POSL_K key;
			T_POSL_D data;
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

BOOL CDB_POSL::Get(T_POSL_K Key, T_POSL_D& rData)
{
	return m_posl.Lookup(Key, rData);
}

int CDB_POSL::GetCount()
{
	return m_posl.GetCount();
}

POSITION CDB_POSL::GetStart()
{
	return m_posl.GetStartPosition();
}

void CDB_POSL::GetNext(POSITION& rNextPosition, T_POSL_K& rKey, T_POSL_D& rData)
{
	m_posl.GetNextAssoc(rNextPosition, rKey, rData);
}
