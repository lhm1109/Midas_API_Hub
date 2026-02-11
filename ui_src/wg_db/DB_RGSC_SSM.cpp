#include "stdafx.h"
#include "DB_RGSC_SSM.h"

#include "DBDoc.h"

CDB_RGSC_SSM::CDB_RGSC_SSM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rgscSsm.InitHashTable(HASHSIZERGSCSSM);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_RGSC_SSM::~CDB_RGSC_SSM()
{

}

void CDB_RGSC_SSM::Add(T_RGSC_SSM_K Key, T_RGSC_SSM_D& rData)
{
	m_rgscSsm.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_RGSC_SSM_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_rgscSsm.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_RGSC_SSM::Del(T_RGSC_SSM_K Key)
{
	T_RGSC_SSM_D Data;
	BOOL bExist = m_rgscSsm.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_rgscSsm.RemoveKey(Key);
	if (!ret) return FALSE;

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_RGSC_SSM_K key;
		T_RGSC_SSM_D data;
		if (m_rgscSsm.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_rgscSsm.GetStartPosition();
			while (pos != NULL)
			{
				m_rgscSsm.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_RGSC_SSM::Get(T_RGSC_SSM_K Key, T_RGSC_SSM_D& rData)
{
	return m_rgscSsm.Lookup(Key, rData);
}

int CDB_RGSC_SSM::GetCount()
{
	return m_rgscSsm.GetCount();
}

POSITION CDB_RGSC_SSM::GetStart()
{
	return m_rgscSsm.GetStartPosition();
}

void CDB_RGSC_SSM::GetNext(POSITION& rNextPosition, T_RGSC_SSM_K& rKey, T_RGSC_SSM_D& rData)
{
	m_rgscSsm.GetNextAssoc(rNextPosition, rKey, rData);
}