#include "stdafx.h"
#include "DB_SISR_USER.h"
#include "DB_ELEM.h"
#include "DB_SISR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SISR_USER::CDB_SISR_USER()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sisrUser.InitHashTable(HASHSIZESISR_USER);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SISR_USER::~CDB_SISR_USER()
{

}

void CDB_SISR_USER::Add(T_SISR_USER_K Key, T_SISR_USER_D& rData, CDB_SISR* pSisr, CDB_ELEM* pElem)
{
	T_SISR_USER_D Data;
	BOOL bExist = m_sisrUser.Lookup(Key, Data);

	T_SISR_D dSisr;
	T_ELEM_D dElem;

	if (bExist)
	{
		if (pSisr) {
			if (pSisr->Get(Data.kSisr, dSisr)) VERIFY(pSisr->DelListItem(Data.kSisr, LT_SISR_USER_CMD, Key));
		}
		if (pElem) {
			if (pElem->Get(Data.kElem, dElem)) VERIFY(pElem->DelListItem(Data.kElem, LT_SISR_USER_CMD, Key));
		}
	}

	m_sisrUser.SetAt(Key, rData);

	if (pSisr) {
		if (pSisr->Get(rData.kSisr, dSisr)) pSisr->AddListItem(rData.kSisr, LT_SISR_USER_CMD, Key);
	}
	if (pElem) {
		if (pElem->Get(rData.kElem, dElem)) pElem->AddListItem(rData.kElem, LT_SISR_USER_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_SISR_USER_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sisrUser.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SISR_USER::Del(T_SISR_USER_K Key, CDB_SISR* pSisr, CDB_ELEM* pElem)
{
	T_SISR_USER_D Data;
	BOOL bExist = m_sisrUser.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_SISR_D dSisr;
	T_ELEM_D dElem;

	BOOL ret = m_sisrUser.RemoveKey(Key);
	if (ret)
	{
		if (pSisr) {
			if (pSisr->Get(Data.kSisr, dSisr)) pSisr->DelListItem(Data.kSisr, LT_SISR_USER_CMD, Key);
		}
		if (pElem) {
			if (pElem->Get(Data.kElem, dElem)) pElem->DelListItem(Data.kElem, LT_SISR_USER_CMD, Key);
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SISR_USER_K key;
		T_SISR_USER_D data;
		if (m_sisrUser.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sisrUser.GetStartPosition();
			while (pos != NULL)
			{
				m_sisrUser.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SISR_USER::Get(T_SISR_USER_K Key, T_SISR_USER_D& rData)
{
	return m_sisrUser.Lookup(Key, rData);
}

int CDB_SISR_USER::GetCount()
{
	return m_sisrUser.GetCount();
}

POSITION CDB_SISR_USER::GetStart()
{
	return m_sisrUser.GetStartPosition();
}

void CDB_SISR_USER::GetNext(POSITION& rNextPosition, T_SISR_USER_K& rKey, T_SISR_USER_D& rData)
{
	m_sisrUser.GetNextAssoc(rNextPosition, rKey, rData);
}