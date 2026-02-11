#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_POSP.h"

#include "DBDoc.h"

CDB_POSP::CDB_POSP()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_posp.InitHashTable(HASHSIZEPOSP);
}

CDB_POSP::~CDB_POSP()
{

}

void CDB_POSP::Add(T_POSP_K Key, T_POSP_D& rData)
{
	T_POSP_D data;
	m_posp.SetAt(Key, rData);
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

BOOL CDB_POSP::Del(T_POSP_K Key)
{
	BOOL ret = m_posp.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_POSP_K key;
			T_POSP_D data;
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

BOOL CDB_POSP::Get(T_POSP_K Key, T_POSP_D& rData)
{
	return m_posp.Lookup(Key, rData);
}

int CDB_POSP::GetCount()
{
	return m_posp.GetCount();
}

POSITION CDB_POSP::GetStart()
{
	return m_posp.GetStartPosition();
}

void CDB_POSP::GetNext(POSITION& rNextPosition, T_POSP_K& rKey, T_POSP_D& rData)
{
	m_posp.GetNextAssoc(rNextPosition, rKey, rData);
}
