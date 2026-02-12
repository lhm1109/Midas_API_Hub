#include "stdafx.h"
#include "DB_REIN_SSM.h"
#include "DB_ELEM.h"

#include "DBDoc.h"

CDB_REIN_SSM::CDB_REIN_SSM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_reinSsm.InitHashTable(HASHSIZEREINSSM);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_REIN_SSM::~CDB_REIN_SSM()
{

}

void CDB_REIN_SSM::Add(T_REIN_SSM_K Key, T_REIN_SSM_D& rData)
{
	m_reinSsm.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_REIN_SSM_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_reinSsm.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_REIN_SSM::Del(T_REIN_SSM_K Key)
{
	T_REIN_SSM_D Data;
	BOOL bExist = m_reinSsm.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_reinSsm.RemoveKey(Key);
	if (!bExist) return FALSE;

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_REIN_SSM_K key;
		T_REIN_SSM_D data;
		if (m_reinSsm.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_reinSsm.GetStartPosition();
			while (pos != NULL)
			{
				m_reinSsm.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_REIN_SSM::Get(T_REIN_SSM_K Key, T_REIN_SSM_D& rData)
{
	return m_reinSsm.Lookup(Key, rData);
}

int CDB_REIN_SSM::GetCount()
{
	return m_reinSsm.GetCount();
}

POSITION CDB_REIN_SSM::GetStart()
{
	return m_reinSsm.GetStartPosition();
}

void CDB_REIN_SSM::GetNext(POSITION& rNextPosition, T_REIN_SSM_K& rKey, T_REIN_SSM_D& rData)
{
	m_reinSsm.GetNextAssoc(rNextPosition, rKey, rData);
}