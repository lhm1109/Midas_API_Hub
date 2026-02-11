#include "stdafx.h"
#include "DB_SIFA_H29.h"

#include "DBDoc.h"

CDB_SIFA_H29::CDB_SIFA_H29()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dataMap.InitHashTable(HASHSIZESIFA_H29);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIFA_H29::~CDB_SIFA_H29()
{

}

void CDB_SIFA_H29::Add(T_SIFA_H29_K Key, T_SIFA_H29_D& rData)
{
	m_dataMap.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_SIFA_H29_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_dataMap.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIFA_H29::Del(T_SIFA_H29_K Key)
{
	T_SIFA_H29_D Data;
	BOOL bExist = m_dataMap.Lookup(Key, Data);
	if (!bExist) return FALSE;
	
	
	BOOL ret = m_dataMap.RemoveKey(Key);
	if (!ret) return FALSE;
	
	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIFA_H29_K key;
		T_SIFA_H29_D data;
		if (m_dataMap.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_dataMap.GetStartPosition();
			while (pos != NULL)
			{
				m_dataMap.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}
	
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIFA_H29::Get(T_SIFA_H29_D& rData)
{
	return m_dataMap.Lookup(1, rData);
}

BOOL CDB_SIFA_H29::Get(T_SIFA_H29_K Key, T_SIFA_H29_D& rData)
{
	return m_dataMap.Lookup(Key, rData);
}

int CDB_SIFA_H29::GetCount()
{
	return m_dataMap.GetCount();
}

POSITION CDB_SIFA_H29::GetStart()
{
	return m_dataMap.GetStartPosition();
}

void CDB_SIFA_H29::GetNext(POSITION& rNextPosition, T_SIFA_H29_K& rKey, T_SIFA_H29_D& rData)
{
	m_dataMap.GetNextAssoc(rNextPosition, rKey, rData);
}