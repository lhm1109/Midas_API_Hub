#include "stdafx.h"
#include "DB_RRSC_SSM.h"

#include "DBDoc.h"

CDB_RRSC_SSM::CDB_RRSC_SSM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rrscSsm.InitHashTable(HASHSIZERRSCSSM);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_RRSC_SSM::~CDB_RRSC_SSM()
{

}

void CDB_RRSC_SSM::Add(T_RRSC_SSM_K Key, T_RRSC_SSM_D& rData)
{
	m_rrscSsm.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_RRSC_SSM_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_rrscSsm.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_RRSC_SSM::Del(T_RRSC_SSM_K Key)
{
	T_RRSC_SSM_D Data;
	BOOL bExist = m_rrscSsm.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_rrscSsm.RemoveKey(Key);
	if (!ret) return FALSE;
	
	if (Key < m_nStartNum)m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_RRSC_SSM_K key;
		T_RRSC_SSM_D data;
		if (m_rrscSsm.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_rrscSsm.GetStartPosition();
			while (pos != NULL)
			{
				m_rrscSsm.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_RRSC_SSM::Get(T_RRSC_SSM_K Key, T_RRSC_SSM_D& rData)
{
	return m_rrscSsm.Lookup(Key, rData);
}

int CDB_RRSC_SSM::GetCount()
{
	return m_rrscSsm.GetCount();
}

POSITION CDB_RRSC_SSM::GetStart()
{
	return m_rrscSsm.GetStartPosition();
}

void CDB_RRSC_SSM::GetNext(POSITION& rNextPosition, T_RRSC_SSM_K& rKey, T_RRSC_SSM_D& rData)
{
	m_rrscSsm.GetNextAssoc(rNextPosition, rKey, rData);
}