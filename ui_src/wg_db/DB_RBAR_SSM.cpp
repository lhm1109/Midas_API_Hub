#include "stdafx.h"
#include "DB_RBAR_SSM.h"
#include "DB_ELEM.h"

#include "DBDoc.h"

CDB_RBAR_SSM::CDB_RBAR_SSM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbar.InitHashTable(HASHSIZERBARSSM);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_RBAR_SSM::~CDB_RBAR_SSM()
{

}

void CDB_RBAR_SSM::Add(T_RBAR_SSM_K Key, T_RBAR_SSM_D& rData, CDB_ELEM* pElem)
{
	T_RBAR_SSM_D Data;
	BOOL bExist = m_rbar.Lookup(Key, Data);

	T_ELEM_D tElem;

	if (bExist)
	{
		if (pElem) {
			if (pElem->Get(Data.kElem, tElem)) VERIFY(pElem->DelListItem(Data.kElem, LT_RBAR_SSM_CMD, Key));
		}
	}

	m_rbar.SetAt(Key, rData);

	if (pElem) {
		pElem->AddListItem(rData.kElem, LT_RBAR_SSM_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_RBAR_SSM_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_rbar.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_RBAR_SSM::Del(T_RBAR_SSM_K Key, CDB_ELEM* pElem)
{
	T_RBAR_SSM_D Data;
	BOOL bExist = m_rbar.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_ELEM_D tElem;

	BOOL ret = m_rbar.RemoveKey(Key);
	if (ret)
	{
		if (pElem) {
			if (pElem->Get(Data.kElem, tElem)) VERIFY(pElem->DelListItem(Data.kElem, LT_RBAR_SSM_CMD, Key));
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_RBAR_SSM_K key;
		T_RBAR_SSM_D data;
		if (m_rbar.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_rbar.GetStartPosition();
			while (pos != NULL)
			{
				m_rbar.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_RBAR_SSM::Get(T_RBAR_SSM_K Key, T_RBAR_SSM_D& rData)
{
	return m_rbar.Lookup(Key, rData);
}

int CDB_RBAR_SSM::GetCount()
{
	return m_rbar.GetCount();
}

POSITION CDB_RBAR_SSM::GetStart()
{
	return m_rbar.GetStartPosition();
}

void CDB_RBAR_SSM::GetNext(POSITION& rNextPosition, T_RBAR_SSM_K& rKey, T_RBAR_SSM_D& rData)
{
	m_rbar.GetNextAssoc(rNextPosition, rKey, rData);
}