#include "stdafx.h"
#include "DB_MPST_SSM.h"

CDB_MPST_SSM::CDB_MPST_SSM()
{
	m_mpstSsm.InitHashTable(HASHSIZEMPST);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_MPST_SSM::~CDB_MPST_SSM()
{

}

void CDB_MPST_SSM::Add(T_MPST_SSM_K Key, T_MPST_SSM_D& rData)
{
	m_mpstSsm.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_MPST_SSM_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_mpstSsm.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_MPST_SSM::Del(T_MPST_SSM_K Key)
{
	T_MPST_SSM_D Data;
	BOOL bExist = m_mpstSsm.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_mpstSsm.RemoveKey(Key);
	if (!ret) return FALSE;

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_MPST_SSM_K key;
		T_MPST_SSM_D data;
		if (m_mpstSsm.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_mpstSsm.GetStartPosition();
			while (pos != NULL)
			{
				m_mpstSsm.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_MPST_SSM::Get(T_MPST_SSM_K Key, T_MPST_SSM_D& rData)
{
	return m_mpstSsm.Lookup(Key, rData);
}

int CDB_MPST_SSM::GetCount()
{
	return m_mpstSsm.GetCount();
}

POSITION CDB_MPST_SSM::GetStart()
{
	return m_mpstSsm.GetStartPosition();
}

void CDB_MPST_SSM::GetNext(POSITION& rNextPosition, T_MPST_SSM_K& rKey, T_MPST_SSM_D& rData)
{
	m_mpstSsm.GetNextAssoc(rNextPosition, rKey, rData);
}