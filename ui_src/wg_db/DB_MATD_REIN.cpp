#include "stdafx.h"
#include "DB_MATD_REIN.h"
#include "DB_MATL.h"

#include "DBDoc.h"

CDB_MATD_REIN::CDB_MATD_REIN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dataMap.InitHashTable(HASHSIZEMATDREIN);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_MATD_REIN::~CDB_MATD_REIN()
{

}

void CDB_MATD_REIN::Add(T_MATD_REIN_K Key, T_MATD_REIN_D& rData, CDB_MATL* pMatl)
{
	T_MATD_REIN_D Data;
	BOOL bExist = m_dataMap.Lookup(Key, Data);
	if(!bExist    && pMatl != NULL)  pMatl->AddListItem(Key, LT_MATD_REIN_CMD, Key);

	m_dataMap.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_MATD_REIN_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_dataMap.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_MATD_REIN::Del(T_MATD_REIN_K Key, CDB_MATL* pMatl)
{
	T_MATD_REIN_D Data;
	BOOL bExist = m_dataMap.Lookup(Key, Data);
	if (!bExist) return FALSE;
	
	BOOL ret = m_dataMap.RemoveKey(Key);
	if (!ret) return FALSE;

	if (bExist && ret)
	{
		if (pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_MATD_REIN_CMD, Key));
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_MATD_REIN_K key;
		T_MATD_REIN_D data;
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
BOOL CDB_MATD_REIN::Get(T_MATD_REIN_K Key, T_MATD_REIN_D& rData)
{
	return m_dataMap.Lookup(Key, rData);
}

int CDB_MATD_REIN::GetCount()
{
	return m_dataMap.GetCount();
}

POSITION CDB_MATD_REIN::GetStart()
{
	return m_dataMap.GetStartPosition();
}

void CDB_MATD_REIN::GetNext(POSITION& rNextPosition, T_MATD_REIN_K& rKey, T_MATD_REIN_D& rData)
{
	m_dataMap.GetNextAssoc(rNextPosition, rKey, rData);
}